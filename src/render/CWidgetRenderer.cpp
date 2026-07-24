#include "CWidgetRenderer.hpp"
#include "CWidgetPassElement.hpp"

#include <src/render/Renderer.hpp>
#include <src/Compositor.hpp>

namespace Hyprlooks {

    void CWidgetRenderer::init(CHyprlandEventBus* eventBus, CHyprlandRenderer* renderer, CSafetyManager* safety) {
        m_eventBus = eventBus;
        m_renderer = renderer;
        m_safety   = safety;

        m_eventBus->subscribeRenderStage([this](eRenderStage stage) { onRenderStage(stage); });
        m_eventBus->subscribeMonitorRemoved([](PHLMONITOR monitor) { removeWidgetTreeForMonitor(monitor); });
    }

    void CWidgetRenderer::shutdown() {
        m_eventBus = nullptr;
        m_renderer = nullptr;
        m_safety   = nullptr;
    }

    void CWidgetRenderer::onRenderStage(eRenderStage stage) {
        if (!m_safety || !m_safety->canRender() || !m_renderer)
            return;

        if (stage != RENDER_POST_WINDOWS)
            return;

        auto pMonitor = g_pHyprRenderer ? g_pHyprRenderer->m_renderData.pMonitor.lock() : nullptr;
        if (!pMonitor)
            return;

        auto result = CCrashIsolation::guard("render.onRenderStage", [&]() {
            auto& tree = widgetTreeForMonitor(pMonitor);
            if (!tree->root() || !tree->root()->isVisible())
                return;

            const bool treeChanged = tree->root()->isDirty();
            tree->relayout();

            const float         scale = m_renderer->monitorScale(pMonitor);

            CRenderCommandQueue queue;
            tree->collectRenderCommands(queue, scale);

            if (queue.empty())
                return;

            queue.sortByPriority();
            if (!m_safety->canBlur())
                queue.filterExpensive(false);

            const bool needsDamage = treeChanged || tree->root()->isDirty();
            auto       bbox        = queue.totalBoundingBox(1.F);
            if (needsDamage && bbox) {
                auto globalBox = *bbox;
                globalBox.x += pMonitor->m_position.x;
                globalBox.y += pMonitor->m_position.y;
                m_renderer->damageBox(globalBox);

                auto lastDamage = tree->lastDamageBox();
                if (lastDamage.w > 0 && lastDamage.h > 0)
                    m_renderer->damageBox(lastDamage);

                tree->setLastDamageBox(globalBox);
            }

            m_renderer->addPassElement(makeUnique<CWidgetPassElement>(std::move(queue), scale));
        });

        if (result.success)
            m_safety->onRenderSuccess();
        else
            m_safety->onRenderFailure("render.onRenderStage");
    }

}
