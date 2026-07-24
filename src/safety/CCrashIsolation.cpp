#include "CCrashIsolation.hpp"

#include <src/debug/log/Logger.hpp>

#include <exception>

namespace Hyprlooks {

CCrashIsolation::SResult CCrashIsolation::guard(const std::string& context, GuardFn fn) {
    try {
        fn();
        return {true, ""};
    } catch (const std::exception& e) {
        Log::logger->log(Log::ERR, "[hyprlooks] crash isolated in {}: {}", context, e.what());
        return {false, e.what()};
    } catch (...) {
        Log::logger->log(Log::ERR, "[hyprlooks] crash isolated in {}: unknown exception", context);
        return {false, "unknown exception"};
    }
}

CCrashIsolation::SResult CCrashIsolation::guard(const std::string& context, GuardFn fn, std::function<SResult()> onSuccess) {
    try {
        fn();
        return onSuccess();
    } catch (const std::exception& e) {
        Log::logger->log(Log::ERR, "[hyprlooks] crash isolated in {}: {}", context, e.what());
        return {false, e.what()};
    } catch (...) {
        Log::logger->log(Log::ERR, "[hyprlooks] crash isolated in {}: unknown exception", context);
        return {false, "unknown exception"};
    }
}

}
