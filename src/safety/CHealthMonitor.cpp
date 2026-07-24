#include "CHealthMonitor.hpp"

namespace Hyprlooks {

void CHealthMonitor::recordFailure(const std::string& subsystem) {
    auto& health = m_health[subsystem];
    auto  now    = std::chrono::steady_clock::now();

    if (health.failures == 0)
        health.firstFailure = now;

    auto age = std::chrono::duration_cast<std::chrono::seconds>(now - health.firstFailure);
    if (age.count() > FAILURE_WINDOW_SECONDS) {
        health.failures      = 1;
        health.firstFailure  = now;
    } else
        health.failures++;
}

void CHealthMonitor::recordSuccess(const std::string& subsystem) {
    auto it = m_health.find(subsystem);
    if (it != m_health.end())
        it->second.failures = 0;
}

bool CHealthMonitor::isSubsystemErrored(const std::string& subsystem) const {
    auto it = m_health.find(subsystem);
    if (it == m_health.end())
        return false;
    return it->second.failures >= MAX_FAILURES_PER_SUBSYSTEM;
}

int CHealthMonitor::failureCount(const std::string& subsystem) const {
    auto it = m_health.find(subsystem);
    if (it == m_health.end())
        return 0;
    return it->second.failures;
}

void CHealthMonitor::reset() {
    m_health.clear();
}

}
