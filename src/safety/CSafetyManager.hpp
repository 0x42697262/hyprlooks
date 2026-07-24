#pragma once

#include <src/defines.hpp>

#include "CSafeMode.hpp"
#include "CWatchdog.hpp"
#include "CCrashIsolation.hpp"
#include "CHealthMonitor.hpp"

#include <string>
#include <functional>

namespace Hyprlooks {

class CSafetyManager {
  public:
    CSafetyManager()  = default;
    ~CSafetyManager() = default;

    void init(std::function<void(const std::string&, eSafeMode)> notifyFn);

    CSafeMode&       safeMode();
    CWatchdog&       watchdog();
    CHealthMonitor&  health();

    void             onRenderFailure(const std::string& context);
    void             onInputFailure(const std::string& context);
    void             onLuaFailure(const std::string& context);
    void             onRenderSuccess();
    void             onInputSuccess();

    void             requestRecovery();
    void             disable();

    bool             canRender() const;
    bool             canBlur() const;
    bool             canAnimate() const;
    bool             canProcessInput() const;

    std::string      statusString() const;

  private:
    CSafeMode      m_safeMode;
    CWatchdog      m_watchdog;
    CHealthMonitor m_health;
};

UP<CSafetyManager>& safetyManager();

}
