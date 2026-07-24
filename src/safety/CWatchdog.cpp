#include "CWatchdog.hpp"

namespace Hyprlooks {

    void CWatchdog::beginRender() {
        m_renderStart = Clock::now();
    }

    void CWatchdog::endRender() {
        m_render.elapsed = Clock::now() - m_renderStart;
    }

    bool CWatchdog::renderExceeded() const {
        return m_render.exceeded();
    }

    void CWatchdog::beginInput() {
        m_inputStart = Clock::now();
    }

    void CWatchdog::endInput() {
        m_input.elapsed = Clock::now() - m_inputStart;
    }

    bool CWatchdog::inputExceeded() const {
        return m_input.exceeded();
    }

}
