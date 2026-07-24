#include "CHoverCommand.hpp"

namespace Hyprlooks {

CHoverCommand::CHoverCommand(WP<IWidget> widget, Hyprutils::Math::Vector2D pos, Callback callback) :
    m_widget(std::move(widget)), m_pos(std::move(pos)), m_callback(std::move(callback)) {
    ;
}

void CHoverCommand::execute() {
    if (m_callback)
        m_callback(m_pos);
}

const char* CHoverCommand::name() const {
    return "CHoverCommand";
}

}
