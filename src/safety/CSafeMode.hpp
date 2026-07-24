#pragma once

#include <string>
#include <functional>
#include <chrono>

namespace Hyprlooks {

    enum class eSafeMode : uint8_t {
        NORMAL    = 0,
        DEGRADED  = 1,
        SAFE_MODE = 2,
        DISABLED  = 3,
    };

    const char* safeModeName(eSafeMode mode);

    class CSafeMode {
      public:
        CSafeMode()  = default;
        ~CSafeMode() = default;

        eSafeMode mode() const;

        void      escalate(eSafeMode toMode, const std::string& reason);
        void      requestRecovery();
        void      notifyCleanFrame();

        bool      canRender() const;
        bool      canBlur() const;
        bool      canAnimate() const;
        bool      canProcessInput() const;
        bool      isDisabled() const;

        void      setNotifyFn(std::function<void(const std::string&, eSafeMode)> fn);

      private:
        void                                               transitionTo(eSafeMode toMode, const std::string& reason);

        eSafeMode                                          m_mode        = eSafeMode::NORMAL;
        int                                                m_cleanFrames = 0;
        std::function<void(const std::string&, eSafeMode)> m_notifyFn;
    };

}
