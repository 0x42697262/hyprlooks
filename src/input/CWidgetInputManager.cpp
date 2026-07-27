#include "CWidgetInputManager.hpp"
#include "commands/CClickCommand.hpp"
#include "commands/CHoverCommand.hpp"
#include "commands/CScrollCommand.hpp"
#include "commands/CKeyPressCommand.hpp"

#include "../state/CNormalState.hpp"
#include "../state/CHoverState.hpp"
#include "../state/CPressedState.hpp"
#include "../widgets/CButton.hpp"
#include "../widgets/CScroll.hpp"
#include "../widgets/CInput.hpp"
#include "../platform/CHyprlandRenderer.hpp"
#include "../platform/CKeyboardTranslator.hpp"

#include <src/Compositor.hpp>

#include <xkbcommon/xkbcommon-keysyms.h>

namespace Hyprlooks {

    void CWidgetInputManager::init(CHyprlandEventBus* eventBus, CHyprlandInput* input, CSafetyManager* safety) {
        m_eventBus = eventBus;
        m_input    = input;
        m_safety   = safety;

        m_eventBus->subscribeMouseMove([this](const Hyprutils::Math::Vector2D& pos, Event::SCallbackInfo& info) { onMouseMove(pos, info); });
        m_eventBus->subscribeMouseButton([this](const IPointer::SButtonEvent& e, Event::SCallbackInfo& info) { onMouseButton(e, info); });
        m_eventBus->subscribeMouseAxis([this](const IPointer::SAxisEvent& e, Event::SCallbackInfo& info) { onMouseAxis(e, info); });
        m_eventBus->subscribeKeyboardKey([this](const IKeyboard::SKeyEvent& e, Event::SCallbackInfo& info) { onKeyboardKey(e, info); });
        m_eventBus->subscribeConfigReloaded([this]() {
            m_hoveredWidget.reset();
            m_pressedWidget.reset();
            m_focusManager.clearFocus();
        });
    }

    void CWidgetInputManager::shutdown() {
        m_eventBus = nullptr;
        m_input    = nullptr;
        m_safety   = nullptr;
        m_commandQueue.clear();
        m_focusManager.clearFocus();
        m_hoveredWidget.reset();
        m_pressedWidget.reset();
    }

    void CWidgetInputManager::flushCommands() {
        if (!m_safety || !m_safety->canProcessInput())
            return;

        auto result = CCrashIsolation::guard("input.flushCommands", [&]() { m_commandQueue.flush(); });

        if (result.success)
            m_safety->onInputSuccess();
        else
            m_safety->onInputFailure("input.flushCommands");
    }

    CFocusManager& CWidgetInputManager::focusManager() {
        return m_focusManager;
    }

    void CWidgetInputManager::setHovered(IWidget* w) {
        // self-pointers are weak ptrs over UP-owned widgets: not lockable, so
        // use get() (returns null once the widget is destroyed) instead of lock().
        auto* previous = m_hoveredWidget.get();
        if (previous == w)
            return;

        if (previous && previous->interactive())
            previous->setState(makeUnique<CNormalState>());

        m_hoveredWidget = w ? w->self() : WP<IWidget>{};

        if (w && w->interactive()) {
            if (m_pressedWidget.get() == w)
                w->setState(makeUnique<CPressedState>());
            else
                w->setState(makeUnique<CHoverState>());
        }
    }

    void CWidgetInputManager::clearHovered() {
        auto* previous = m_hoveredWidget.get();
        if (previous && previous->interactive())
            previous->setState(makeUnique<CNormalState>());
        m_hoveredWidget.reset();
    }

    void CWidgetInputManager::setInputFocus(IWidget* w) {
        auto* previous = m_focusManager.focused().get();
        if (previous == w)
            return;

        if (previous && previous->type() == eWidgetType::INPUT)
            static_cast<CInput*>(previous)->setEditing(false);

        if (w && w->type() == eWidgetType::INPUT) {
            m_focusManager.setFocused(w->self());
            static_cast<CInput*>(w)->setEditing(true);
        } else
            m_focusManager.clearFocus();

        static CHyprlandRenderer renderer;
        renderer.scheduleAllFrames();
    }

    void CWidgetInputManager::onMouseMove(const Hyprutils::Math::Vector2D& pos, Event::SCallbackInfo& info) {
        if (!m_safety || !m_safety->canProcessInput() || !m_input)
            return;

        CCrashIsolation::guard("input.onMouseMove", [&]() {
            const auto cursorPos = m_input->cursorPosition();

            auto&      treeRef = widgetTreeForMonitor(g_pCompositor->getMonitorFromCursor());
            auto*      tree    = treeRef.get();
            if (!tree || !tree->root())
                return;

            IWidget* hit = m_hitTester.hitTest(tree, cursorPos);
            if (hit) {
                info.cancelled = true;
                setHovered(hit);
            } else
                clearHovered();
        });
    }

    void CWidgetInputManager::onMouseButton(const IPointer::SButtonEvent& e, Event::SCallbackInfo& info) {
        if (!m_safety || !m_safety->canProcessInput() || !m_input)
            return;

        CCrashIsolation::guard("input.onMouseButton", [&]() {
            const auto cursorPos = m_input->cursorPosition();

            auto&      treeRef = widgetTreeForMonitor(g_pCompositor->getMonitorFromCursor());
            auto*      tree    = treeRef.get();
            if (!tree || !tree->root())
                return;

            IWidget* hit = m_hitTester.hitTest(tree, cursorPos);
            if (!hit)
                return;

            info.cancelled = true;

            if (e.state == WL_POINTER_BUTTON_STATE_PRESSED) {
                m_pressedWidget = hit->self();
                hit->setState(makeUnique<CPressedState>());
            } else {
                auto* pressed = m_pressedWidget.get();
                m_pressedWidget.reset();

                if (pressed == hit) {
                    hit->setState(makeUnique<CHoverState>());
                    // clicking an input focuses it; clicking anything else clears focus
                    setInputFocus(hit);
                    if (hit->type() == eWidgetType::BUTTON) {
                        auto* btn = static_cast<CButton*>(hit);
                        m_commandQueue.enqueue(makeUnique<CClickCommand>(hit->self(), e.button, cursorPos, [btn]() { btn->fireClick(); }));
                        flushCommands();
                    }
                } else if (pressed && pressed->interactive()) {
                    pressed->setState(makeUnique<CNormalState>());
                }
            }
        });
    }

    void CWidgetInputManager::onMouseAxis(const IPointer::SAxisEvent& e, Event::SCallbackInfo& info) {
        if (!m_safety || !m_safety->canProcessInput() || !m_input)
            return;

        CCrashIsolation::guard("input.onMouseAxis", [&]() {
            const auto cursorPos = m_input->cursorPosition();

            auto&      treeRef = widgetTreeForMonitor(g_pCompositor->getMonitorFromCursor());
            auto*      tree    = treeRef.get();
            if (!tree || !tree->root())
                return;

            // route vertical wheel to a scrollable container under the cursor
            if (auto* scrollable = m_hitTester.scrollableAt(tree, cursorPos)) {
                info.cancelled = true;
                if (e.axis == WL_POINTER_AXIS_VERTICAL_SCROLL) {
                    static constexpr double SCROLL_SPEED = 3.0;
                    static_cast<CScroll*>(scrollable)->scrollBy(e.delta * SCROLL_SPEED);
                    static CHyprlandRenderer renderer;
                    renderer.scheduleAllFrames();
                }
                return;
            }

            // otherwise just consume the event if it lands on our UI
            if (m_hitTester.hitTest(tree, cursorPos))
                info.cancelled = true;
        });
    }

    void CWidgetInputManager::onKeyboardKey(const IKeyboard::SKeyEvent& e, Event::SCallbackInfo& info) {
        if (!m_safety || !m_safety->canProcessInput())
            return;

        CCrashIsolation::guard("input.onKeyboardKey", [&]() {
            const bool pressed = e.state == WL_KEYBOARD_KEY_STATE_PRESSED;

            // always feed the translator so modifier state (shift, caps) stays correct
            keyboardTranslator()->updateKey(e.keycode, pressed);

            auto* focused = m_focusManager.focused().get();
            if (!focused || focused->type() != eWidgetType::INPUT)
                return; // nothing is capturing text; let the key through

            info.cancelled = true; // swallow keys while a field is focused
            if (!pressed)
                return;

            auto*          input = static_cast<CInput*>(focused);
            const uint32_t sym   = keyboardTranslator()->keysym(e.keycode);

            if (sym == XKB_KEY_Escape) {
                setInputFocus(nullptr);
            } else if (sym == XKB_KEY_BackSpace) {
                input->backspace();
            } else if (sym == XKB_KEY_Return || sym == XKB_KEY_KP_Enter) {
                input->submit();
            } else {
                const std::string utf8 = keyboardTranslator()->utf8(e.keycode);
                if (!utf8.empty() && static_cast<unsigned char>(utf8[0]) >= 0x20)
                    input->insertText(utf8);
            }

            static CHyprlandRenderer renderer;
            renderer.scheduleAllFrames();
        });
    }

}
