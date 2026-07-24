#pragma once

#include "../IInputCommand.hpp"
#include "../../core/IWidget.hpp"

#include <src/devices/IKeyboard.hpp>

#include <functional>

namespace Hyprlooks {

class CKeyPressCommand : public IInputCommand {
  public:
    using Callback = std::function<void(uint32_t keycode, bool pressed)>;

    CKeyPressCommand(WP<IWidget> widget, uint32_t keycode, bool pressed, Callback callback);

    virtual void        execute() override;
    virtual const char* name() const override;

  private:
    WP<IWidget> m_widget;
    uint32_t    m_keycode;
    bool        m_pressed;
    Callback    m_callback;
};

}
