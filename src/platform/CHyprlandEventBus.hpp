#pragma once

#include <src/event/EventBus.hpp>
#include <src/SharedDefs.hpp>

#include <hyprutils/signal/Signal.hpp>

#include <functional>

class CHyprlandEventBus {
  public:
    using RenderStageCallback     = std::function<void(eRenderStage)>;
    using MouseMoveCallback       = std::function<void(const Hyprutils::Math::Vector2D&, Event::SCallbackInfo&)>;
    using MouseButtonCallback     = std::function<void(const IPointer::SButtonEvent&, Event::SCallbackInfo&)>;
    using MouseAxisCallback       = std::function<void(const IPointer::SAxisEvent&, Event::SCallbackInfo&)>;
    using KeyboardKeyCallback     = std::function<void(const IKeyboard::SKeyEvent&, Event::SCallbackInfo&)>;
    using WorkspaceActiveCallback = std::function<void(PHLWORKSPACE)>;
    using MonitorFocusedCallback  = std::function<void(PHLMONITOR)>;
    using MonitorRemovedCallback  = std::function<void(PHLMONITOR)>;
    using ConfigPreReloadCallback = std::function<void()>;
    using ConfigReloadedCallback  = std::function<void()>;

    CHyprlandEventBus()  = default;
    ~CHyprlandEventBus() = default;

    void subscribeRenderStage(RenderStageCallback cb);
    void subscribeMouseMove(MouseMoveCallback cb);
    void subscribeMouseButton(MouseButtonCallback cb);
    void subscribeMouseAxis(MouseAxisCallback cb);
    void subscribeKeyboardKey(KeyboardKeyCallback cb);
    void subscribeWorkspaceActive(WorkspaceActiveCallback cb);
    void subscribeMonitorFocused(MonitorFocusedCallback cb);
    void subscribeMonitorRemoved(MonitorRemovedCallback cb);
    void subscribeConfigPreReload(ConfigPreReloadCallback cb);
    void subscribeConfigReloaded(ConfigReloadedCallback cb);

    void disconnectAll();

  private:
    Hyprutils::Signal::CHyprSignalListener m_renderStageListener;
    Hyprutils::Signal::CHyprSignalListener m_mouseMoveListener;
    Hyprutils::Signal::CHyprSignalListener m_mouseButtonListener;
    Hyprutils::Signal::CHyprSignalListener m_mouseAxisListener;
    Hyprutils::Signal::CHyprSignalListener m_keyboardKeyListener;
    Hyprutils::Signal::CHyprSignalListener m_workspaceActiveListener;
    Hyprutils::Signal::CHyprSignalListener m_monitorFocusedListener;
    Hyprutils::Signal::CHyprSignalListener m_monitorRemovedListener;
    Hyprutils::Signal::CHyprSignalListener m_configPreReloadListener;
    Hyprutils::Signal::CHyprSignalListener m_configReloadedListener;
};
