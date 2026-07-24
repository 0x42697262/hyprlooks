#include "CClickCommand.hpp"

namespace Hyprlooks {

    CClickCommand::CClickCommand(WP<IWidget> widget, uint32_t button, Hyprutils::Math::Vector2D pos, Callback callback) :
        m_widget(std::move(widget)), m_button(button), m_pos(std::move(pos)), m_callback(std::move(callback)) {
        ;
    }

    void CClickCommand::execute() {
        // m_widget is a weak ptr over a UP-owned widget; such a weak ptr is
        // deliberately non-lockable in hyprutils. Use valid() to confirm the
        // widget still exists rather than lock() (which always returns empty).
        if (m_widget.valid() && m_callback)
            m_callback();
    }

    const char* CClickCommand::name() const {
        return "CClickCommand";
    }

}
