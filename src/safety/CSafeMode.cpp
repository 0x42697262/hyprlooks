#include "CSafeMode.hpp"

namespace Hyprlooks {

const char* safeModeName(eSafeMode mode) {
    switch (mode) {
        case eSafeMode::NORMAL: return "NORMAL";
        case eSafeMode::DEGRADED: return "DEGRADED";
        case eSafeMode::SAFE_MODE: return "SAFE_MODE";
        case eSafeMode::DISABLED: return "DISABLED";
    }
    return "UNKNOWN";
}

eSafeMode CSafeMode::mode() const {
    return m_mode;
}

void CSafeMode::escalate(eSafeMode toMode, const std::string& reason) {
    if (static_cast<int>(toMode) <= static_cast<int>(m_mode))
        return;
    transitionTo(toMode, reason);
}

void CSafeMode::requestRecovery() {
    if (m_mode == eSafeMode::DISABLED)
        return;
    if (m_mode == eSafeMode::SAFE_MODE) {
        transitionTo(eSafeMode::DEGRADED, "manual recovery requested");
        m_cleanFrames = 0;
        return;
    }
}

void CSafeMode::notifyCleanFrame() {
    if (m_mode != eSafeMode::DEGRADED)
        return;

    m_cleanFrames++;
    if (m_cleanFrames >= 3) {
        transitionTo(eSafeMode::NORMAL, "auto-recovery: 3 consecutive clean frames");
        m_cleanFrames = 0;
    }
}

bool CSafeMode::canRender() const {
    return m_mode < eSafeMode::SAFE_MODE;
}

bool CSafeMode::canBlur() const {
    return m_mode == eSafeMode::NORMAL;
}

bool CSafeMode::canAnimate() const {
    return m_mode == eSafeMode::NORMAL;
}

bool CSafeMode::canProcessInput() const {
    return m_mode < eSafeMode::SAFE_MODE;
}

bool CSafeMode::isDisabled() const {
    return m_mode == eSafeMode::DISABLED;
}

void CSafeMode::setNotifyFn(std::function<void(const std::string&, eSafeMode)> fn) {
    m_notifyFn = std::move(fn);
}

void CSafeMode::transitionTo(eSafeMode toMode, const std::string& reason) {
    if (toMode == m_mode)
        return;

    m_mode         = toMode;
    m_cleanFrames  = 0;

    if (m_notifyFn)
        m_notifyFn(reason, toMode);
}

}
