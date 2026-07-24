#include "CClickCommand.hpp"

namespace Hyprlooks {

    CClickCommand::CClickCommand(WP<IWidget> widget, uint32_t button, Hyprutils::Math::Vector2D pos, Callback callback) :
        m_widget(std::move(widget)), m_button(button), m_pos(std::move(pos)), m_callback(std::move(callback)) {
        ;
    }

    void CClickCommand::execute() {
        auto widget = m_widget.lock();
        if (widget && m_callback)
            m_callback();
    }

    const char* CClickCommand::name() const {
        return "CClickCommand";
    }

}
