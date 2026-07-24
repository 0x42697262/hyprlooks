#include "CWidgetTree.hpp"
#include "../render/IRenderCommand.hpp"
#include "../render/CRenderCommandQueue.hpp"

#include <src/helpers/Monitor.hpp>

namespace Hyprlooks {

    void CWidgetTree::setMonitor(PHLMONITOR pMonitor) {
        m_monitor       = pMonitor;
        m_needsRelayout = true;
    }

    PHLMONITOR CWidgetTree::monitor() const {
        return m_monitor;
    }

    void CWidgetTree::setRoot(UP<IWidget> root) {
        m_root = std::move(root);
        if (m_root)
            m_root->setSelf(WP<IWidget>{m_root});
        m_needsRelayout = true;
    }

    IWidget* CWidgetTree::root() const {
        return m_root.get();
    }

    void CWidgetTree::markAllDirty() {
        m_needsRelayout = true;
    }

    void CWidgetTree::collectDirtyWidgets(std::vector<IWidget*>& out) {
        if (m_root)
            collectDirtyRecursive(m_root.get(), out);
    }

    void CWidgetTree::collectDirtyRecursive(IWidget* w, std::vector<IWidget*>& out) {
        if (!w)
            return;
        if (w->isDirty())
            out.push_back(w);
    }

    void CWidgetTree::relayout() {
        if (!m_root || !m_monitor)
            return;

        if (!m_needsRelayout && !m_root->isDirty())
            return;

        const auto box = Hyprutils::Math::CBox{0, 0, m_monitor->m_size.x, m_monitor->m_size.y};
        relayoutRecursive(m_root.get(), box);
        m_needsRelayout = false;
    }

    void CWidgetTree::relayoutRecursive(IWidget* w, const Hyprutils::Math::CBox& available) {
        if (!w)
            return;
        w->layout(available);
    }

    void CWidgetTree::collectRenderCommands(CRenderCommandQueue& queue, float monitorScale) {
        if (m_root && m_root->isVisible())
            collectRenderCommandsRecursive(m_root.get(), queue, monitorScale);
    }

    void CWidgetTree::collectRenderCommandsRecursive(IWidget* w, CRenderCommandQueue& queue, float monitorScale) {
        if (!w || !w->isVisible())
            return;

        auto commands = w->buildRenderCommands(monitorScale);
        for (auto& cmd : commands)
            queue.add(std::move(cmd));
    }

    Hyprutils::Math::CBox CWidgetTree::lastDamageBox() const {
        return m_lastDamage;
    }

    void CWidgetTree::setLastDamageBox(const Hyprutils::Math::CBox& box) {
        m_lastDamage = box;
    }

    static std::unordered_map<MONITORID, UP<CWidgetTree>>& treeMap() {
        static std::unordered_map<MONITORID, UP<CWidgetTree>> map;
        return map;
    }

    UP<CWidgetTree>& widgetTreeForMonitor(PHLMONITOR pMonitor) {
        auto& map = treeMap();
        if (!pMonitor)
            return treeMap()[MONITOR_INVALID];
        auto id = pMonitor->m_id;
        auto it = map.find(id);
        if (it == map.end()) {
            auto [inserted, _] = map.emplace(id, makeUnique<CWidgetTree>());
            inserted->second->setMonitor(pMonitor);
            return inserted->second;
        }
        return it->second;
    }

    void removeWidgetTreeForMonitor(PHLMONITOR pMonitor) {
        if (!pMonitor)
            return;
        treeMap().erase(pMonitor->m_id);
    }

}
