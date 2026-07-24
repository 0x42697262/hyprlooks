#pragma once

#include "../platform/CHyprlandEventBus.hpp"
#include "../platform/CHyprlandRenderer.hpp"
#include "../core/CWidgetTree.hpp"
#include "../render/CRenderCommandQueue.hpp"
#include "../safety/CSafetyManager.hpp"

namespace Hyprlooks {

class CWidgetRenderer {
  public:
    CWidgetRenderer()  = default;
    ~CWidgetRenderer() = default;

    void init(CHyprlandEventBus* eventBus, CHyprlandRenderer* renderer, CSafetyManager* safety);
    void shutdown();

  private:
    void onRenderStage(eRenderStage stage);

    CHyprlandEventBus*  m_eventBus  = nullptr;
    CHyprlandRenderer*  m_renderer  = nullptr;
    CSafetyManager*     m_safety    = nullptr;
};

}
