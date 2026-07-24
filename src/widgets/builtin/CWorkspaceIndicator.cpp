#include "CWorkspaceIndicator.hpp"
#include "../../layout/CFlexLayout.hpp"
#include "../../theme/CThemeDefaults.hpp"
#include "../../platform/CHyprlandConfig.hpp"

#include <src/Compositor.hpp>
#include <src/plugins/PluginAPI.hpp>
#include <src/event/EventBus.hpp>
#include <src/debug/log/Logger.hpp>
#include <src/helpers/Monitor.hpp>

#include <algorithm>
#include <format>

namespace Hyprlooks {

    CWorkspaceIndicator::CWorkspaceIndicator() {
        m_style = CThemeDefaults::containerStyle();

        SFlexOptions opts;
        opts.direction = eFlexDirection::ROW;
        opts.justify   = eFlexJustify::START;
        opts.align     = eFlexAlign::CENTER;
        opts.mainGap   = 4.F;
        setLayoutStrategy(makeUnique<CFlexLayout>(opts));

        m_workspaceListener = Event::bus()->m_events.workspace.active.listen([this](PHLWORKSPACE) { onWorkspaceActive(); });

        rebuild();
    }

    void CWorkspaceIndicator::setMonitorName(const std::string& name) {
        m_monitorName = name;
        rebuild();
    }

    void CWorkspaceIndicator::setActiveBgColor(const CHyprColor& c) {
        m_activeBgColor = c;
        rebuild();
    }

    void CWorkspaceIndicator::setInactiveFgColor(const CHyprColor& c) {
        m_inactiveFgColor = c;
        rebuild();
    }

    eWidgetType CWorkspaceIndicator::type() const {
        return eWidgetType::WORKSPACE_INDICATOR;
    }

    const char* CWorkspaceIndicator::typeName() const {
        return "CWorkspaceIndicator";
    }

    void CWorkspaceIndicator::onWorkspaceActive() {
        rebuild();
    }

    void CWorkspaceIndicator::rebuild() {
        clearChildren();

        if (!g_pCompositor)
            return;

        auto workspaces = g_pCompositor->getWorkspacesCopy();

        std::sort(workspaces.begin(), workspaces.end(), [](const PHLWORKSPACE& a, const PHLWORKSPACE& b) { return a->m_id < b->m_id; });

        for (const auto& ws : workspaces) {
            if (ws->m_isSpecialWorkspace)
                continue;

            if (!m_monitorName.empty()) {
                auto mon = ws->m_monitor.lock();
                if (!mon || mon->m_name != m_monitorName)
                    continue;
            }

            auto        button   = makeUnique<CButton>();
            auto        monitor  = ws->m_monitor.lock();
            bool        isActive = monitor && monitor->m_activeWorkspace && monitor->m_activeWorkspace->m_id == ws->m_id;

            std::string text;
            if (ws->m_name.empty() || ws->m_name == std::to_string(ws->m_id))
                text = std::to_string(ws->m_id);
            else
                text = ws->m_name;

            button->setText(text);

            SWidgetStyle style;
            style.fgColor      = isActive ? CHyprColor{1.F, 1.F, 1.F, 1.F} : m_inactiveFgColor;
            style.bgColor      = isActive ? m_activeBgColor : CHyprColor{0.F, 0.F, 0.F, 0.F};
            style.fontSize     = 13.F;
            style.padding      = {6.F, 2.F};
            style.borderRadius = {4.F, 4.F, 4.F, 4.F};
            button->setStyle(style);

            const auto workspaceID = ws->m_id;
            button->onClick([workspaceID]() {
                CHyprlandConfig config;
                config.focusWorkspace(workspaceID);
            });

            addChild(std::move(button));
        }

        markDirty();
    }

}
