#include "CScrollCommand.hpp"

namespace Hyprlooks {

    CScrollCommand::CScrollCommand(WP<IWidget> widget, int direction, double delta, Callback callback) :
        m_widget(std::move(widget)), m_direction(direction), m_delta(delta), m_callback(std::move(callback)) {
        ;
    }

    void CScrollCommand::execute() {
        if (m_callback)
            m_callback(m_direction, m_delta);
    }

    const char* CScrollCommand::name() const {
        return "CScrollCommand";
    }

}
