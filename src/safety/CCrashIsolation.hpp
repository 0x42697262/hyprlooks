#pragma once

#include <string>
#include <functional>
#include <optional>

namespace Hyprlooks {

class CCrashIsolation {
  public:
    struct SResult {
        bool        success = true;
        std::string error;
    };

    using GuardFn = std::function<void()>;

    static SResult guard(const std::string& context, GuardFn fn);
    static SResult guard(const std::string& context, GuardFn fn, std::function<SResult()> onSuccess);
};

}
