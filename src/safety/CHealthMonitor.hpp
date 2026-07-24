#pragma once

#include <string>
#include <chrono>
#include <unordered_map>
#include <cstdint>

namespace Hyprlooks {

    class CHealthMonitor {
      public:
        CHealthMonitor()  = default;
        ~CHealthMonitor() = default;

        void                 recordFailure(const std::string& subsystem);
        void                 recordSuccess(const std::string& subsystem);

        bool                 isSubsystemErrored(const std::string& subsystem) const;
        int                  failureCount(const std::string& subsystem) const;

        void                 reset();

        static constexpr int MAX_FAILURES_PER_SUBSYSTEM = 5;
        static constexpr int FAILURE_WINDOW_SECONDS     = 60;

      private:
        struct SSubsystemHealth {
            int                                   failures = 0;
            std::chrono::steady_clock::time_point firstFailure;
        };

        std::unordered_map<std::string, SSubsystemHealth> m_health;
    };

}
