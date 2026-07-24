#include "CHyprlandEventBus.hpp"

void CHyprlandEventBus::subscribeRenderStage(RenderStageCallback cb) {
    m_renderStageListener = Event::bus()->m_events.render.stage.listen([cb](eRenderStage stage) { cb(stage); });
}

void CHyprlandEventBus::subscribeMouseMove(MouseMoveCallback cb) {
    m_mouseMoveListener = Event::bus()->m_events.input.mouse.move.listen([cb](const Hyprutils::Math::Vector2D& pos, Event::SCallbackInfo& info) { cb(pos, info); });
}

void CHyprlandEventBus::subscribeMouseButton(MouseButtonCallback cb) {
    m_mouseButtonListener = Event::bus()->m_events.input.mouse.button.listen([cb](const IPointer::SButtonEvent& e, Event::SCallbackInfo& info) { cb(e, info); });
}

void CHyprlandEventBus::subscribeMouseAxis(MouseAxisCallback cb) {
    m_mouseAxisListener = Event::bus()->m_events.input.mouse.axis.listen([cb](const IPointer::SAxisEvent& e, Event::SCallbackInfo& info) { cb(e, info); });
}

void CHyprlandEventBus::subscribeKeyboardKey(KeyboardKeyCallback cb) {
    m_keyboardKeyListener = Event::bus()->m_events.input.keyboard.key.listen([cb](const IKeyboard::SKeyEvent& e, Event::SCallbackInfo& info) { cb(e, info); });
}

void CHyprlandEventBus::subscribeWorkspaceActive(WorkspaceActiveCallback cb) {
    m_workspaceActiveListener = Event::bus()->m_events.workspace.active.listen([cb](PHLWORKSPACE ws) { cb(ws); });
}

void CHyprlandEventBus::subscribeMonitorFocused(MonitorFocusedCallback cb) {
    m_monitorFocusedListener = Event::bus()->m_events.monitor.focused.listen([cb](PHLMONITOR mon) { cb(mon); });
}

void CHyprlandEventBus::subscribeConfigReloaded(ConfigReloadedCallback cb) {
    m_configReloadedListener = Event::bus()->m_events.config.reloaded.listen([cb]() { cb(); });
}

void CHyprlandEventBus::disconnectAll() {
    m_renderStageListener.reset();
    m_mouseMoveListener.reset();
    m_mouseButtonListener.reset();
    m_mouseAxisListener.reset();
    m_keyboardKeyListener.reset();
    m_workspaceActiveListener.reset();
    m_monitorFocusedListener.reset();
    m_configReloadedListener.reset();
}
