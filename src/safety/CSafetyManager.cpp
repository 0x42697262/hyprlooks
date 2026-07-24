#include "CSafetyManager.hpp"

#include <src/debug/log/Logger.hpp>

#include <format>

namespace Hyprlooks {

    void CSafetyManager::init(std::function<void(const std::string&, eSafeMode)> notifyFn) {
        m_safeMode.setNotifyFn(std::move(notifyFn));
    }

    CSafeMode& CSafetyManager::safeMode() {
        return m_safeMode;
    }

    CWatchdog& CSafetyManager::watchdog() {
        return m_watchdog;
    }

    CHealthMonitor& CSafetyManager::health() {
        return m_health;
    }

    void CSafetyManager::onRenderFailure(const std::string& context) {
        m_health.recordFailure("render");
        Log::logger->log(Log::WARN, "[hyprlooks] render failure in {}: count={}", context, m_health.failureCount("render"));

        if (m_health.isSubsystemErrored("render"))
            m_safeMode.escalate(eSafeMode::DEGRADED, "render subsystem errored");
    }

    void CSafetyManager::onInputFailure(const std::string& context) {
        m_health.recordFailure("input");
        Log::logger->log(Log::WARN, "[hyprlooks] input failure in {}: count={}", context, m_health.failureCount("input"));

        if (m_health.isSubsystemErrored("input"))
            m_safeMode.escalate(eSafeMode::DEGRADED, "input subsystem errored");
    }

    void CSafetyManager::onLuaFailure(const std::string& context) {
        m_health.recordFailure("lua");
        Log::logger->log(Log::WARN, "[hyprlooks] lua failure in {}: count={}", context, m_health.failureCount("lua"));
    }

    void CSafetyManager::onRenderSuccess() {
        m_health.recordSuccess("render");
        m_safeMode.notifyCleanFrame();
    }

    void CSafetyManager::onInputSuccess() {
        m_health.recordSuccess("input");
    }

    void CSafetyManager::requestRecovery() {
        m_safeMode.requestRecovery();
    }

    void CSafetyManager::disable() {
        m_safeMode.escalate(eSafeMode::DISABLED, "manual disable");
    }

    bool CSafetyManager::canRender() const {
        return m_safeMode.canRender();
    }

    bool CSafetyManager::canBlur() const {
        return m_safeMode.canBlur();
    }

    bool CSafetyManager::canAnimate() const {
        return m_safeMode.canAnimate();
    }

    bool CSafetyManager::canProcessInput() const {
        return m_safeMode.canProcessInput();
    }

    std::string CSafetyManager::statusString() const {
        return std::format("mode={} render_failures={} input_failures={} lua_failures={}", safeModeName(m_safeMode.mode()), m_health.failureCount("render"),
                           m_health.failureCount("input"), m_health.failureCount("lua"));
    }

    UP<CSafetyManager>& safetyManager() {
        static UP<CSafetyManager> mgr = makeUnique<CSafetyManager>();
        return mgr;
    }

}
