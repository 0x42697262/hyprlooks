#include "CWidgetInputManager.hpp"
#include "commands/CClickCommand.hpp"
#include "commands/CHoverCommand.hpp"
#include "commands/CScrollCommand.hpp"
#include "commands/CKeyPressCommand.hpp"

#include "../state/CNormalState.hpp"
#include "../state/CHoverState.hpp"
#include "../state/CPressedState.hpp"
#include "../widgets/CButton.hpp"

#include <src/Compositor.hpp>

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

            IWidget* hit = m_hitTester.hitTest(tree, cursorPos);
            if (hit)
                info.cancelled = true;
        });
    }

    void CWidgetInputManager::onKeyboardKey(const IKeyboard::SKeyEvent& e, Event::SCallbackInfo& info) {
        if (!m_safety || !m_safety->canProcessInput())
            return;

        CCrashIsolation::guard("input.onKeyboardKey", [&]() {
            if (m_focusManager.hasFocus())
                info.cancelled = true;
        });
    }

}
