#pragma once

#include "../platform/CHyprlandEventBus.hpp"
#include "../platform/CHyprlandInput.hpp"
#include "../core/CWidgetTree.hpp"
#include "../input/CInputCommandQueue.hpp"
#include "../input/CHitTester.hpp"
#include "../input/CFocusManager.hpp"
#include "../safety/CSafetyManager.hpp"

namespace Hyprlooks {

class CWidgetInputManager {
  public:
    CWidgetInputManager()  = default;
    ~CWidgetInputManager() = default;

    void init(CHyprlandEventBus* eventBus, CHyprlandInput* input, CSafetyManager* safety);
    void shutdown();

    void         flushCommands();

    CFocusManager& focusManager();

  private:
    void onMouseMove(const Hyprutils::Math::Vector2D& pos, Event::SCallbackInfo& info);
    void onMouseButton(const IPointer::SButtonEvent& e, Event::SCallbackInfo& info);
    void onMouseAxis(const IPointer::SAxisEvent& e, Event::SCallbackInfo& info);
    void onKeyboardKey(const IKeyboard::SKeyEvent& e, Event::SCallbackInfo& info);

    CHyprlandEventBus*   m_eventBus  = nullptr;
    CHyprlandInput*      m_input     = nullptr;
    CSafetyManager*      m_safety    = nullptr;
    CHitTester           m_hitTester;
    CInputCommandQueue   m_commandQueue;
    CFocusManager        m_focusManager;
};

}
