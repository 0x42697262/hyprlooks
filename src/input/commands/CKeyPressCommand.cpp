#include "CKeyPressCommand.hpp"

namespace Hyprlooks {

CKeyPressCommand::CKeyPressCommand(WP<IWidget> widget, uint32_t keycode, bool pressed, Callback callback) :
    m_widget(std::move(widget)), m_keycode(keycode), m_pressed(pressed), m_callback(std::move(callback)) {
    ;
}

void CKeyPressCommand::execute() {
    if (m_callback)
        m_callback(m_keycode, m_pressed);
}

const char* CKeyPressCommand::name() const {
    return "CKeyPressCommand";
}

}
