#include "globals.hpp"

#include <src/defines.hpp>

#include "platform/CHyprlandRenderer.hpp"
#include "platform/CHyprlandInput.hpp"
#include "platform/CHyprlandEventBus.hpp"
#include "platform/CHyprlandConfig.hpp"
#include "platform/CHyprlandReservation.hpp"

#include "core/CWidgetRegistry.hpp"
#include "core/CWidgetTree.hpp"

#include "safety/CSafetyManager.hpp"

#include "render/CWidgetRenderer.hpp"

#include "input/CWidgetInputManager.hpp"

#include "lua/CLuaAPI.hpp"
#include "lua/CLuaWidgetFactory.hpp"

#include "theme/CTheme.hpp"

#include <src/debug/log/Logger.hpp>
#include <src/SharedDefs.hpp>

#include <stdexcept>

using namespace Hyprlooks;

static UP<CHyprlandEventBus>& eventBus() {
    static auto p = makeUnique<CHyprlandEventBus>();
    return p;
}
static UP<CHyprlandRenderer>& renderer() {
    static auto p = makeUnique<CHyprlandRenderer>();
    return p;
}
static UP<CHyprlandInput>& input() {
    static auto p = makeUnique<CHyprlandInput>();
    return p;
}
static UP<CHyprlandConfig>& config() {
    static auto p = makeUnique<CHyprlandConfig>();
    return p;
}
static UP<CWidgetRenderer>& widgetRenderer() {
    static auto p = makeUnique<CWidgetRenderer>();
    return p;
}
static UP<CWidgetInputManager>& inputManager() {
    static auto p = makeUnique<CWidgetInputManager>();
    return p;
}

static SDispatchResult dispatchRecover(std::string) {
    safetyManager()->requestRecovery();
    config()->notify("Recovery requested", 3000, CHyprColor{0}, ICON_INFO);
    return {false, true, ""};
}

static SDispatchResult dispatchStatus(std::string) {
    config()->notify(safetyManager()->statusString(), 5000, CHyprColor{0}, ICON_INFO);
    return {false, true, ""};
}

static SDispatchResult dispatchDisable(std::string) {
    safetyManager()->disable();
    config()->notify("Plugin disabled", 3000, CHyprColor{0}, ICON_WARNING);
    return {false, true, ""};
}

static SDispatchResult dispatchSafeMode(std::string) {
    safetyManager()->safeMode().escalate(eSafeMode::SAFE_MODE, "manual safe mode");
    config()->notify("Safe mode activated", 3000, CHyprColor{0}, ICON_WARNING);
    return {false, true, ""};
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    auto& safety = safetyManager();
    safety->init(
        [&](const std::string& reason, eSafeMode mode) { config()->notify(std::string{"Safe mode: "} + safeModeName(mode) + " — " + reason, 10000, CHyprColor{0}, ICON_WARNING); });

    auto result = CCrashIsolation::guard("PLUGIN_INIT", [&]() {
        if (!luaAPI()->init())
            throw std::runtime_error("hyprlooks: Lua API init failed");

        eventBus()->subscribeConfigPreReload([]() {
            reservation()->clearAll();
            clearWidgetTrees();
            luaAPI()->onPreReload();
            renderer()->scheduleAllFrames();
        });
        widgetRenderer()->init(eventBus().get(), renderer().get(), safety.get());
        inputManager()->init(eventBus().get(), input().get(), safety.get());

        config()->registerDispatcher("hyprlooks_recover", dispatchRecover);
        config()->registerDispatcher("hyprlooks_status", dispatchStatus);
        config()->registerDispatcher("hyprlooks_disable", dispatchDisable);
        config()->registerDispatcher("hyprlooks_safe_mode", dispatchSafeMode);

        Log::logger->log(Log::INFO, "[hyprlooks] initialized successfully");
    });

    if (!result.success) {
        HyprlandAPI::addNotificationV2(
            handle, {{"text", std::string{"[hyprlooks] failed to initialize: "} + result.error}, {"time", uint64_t{10000}}, {"color", CHyprColor{0}}, {"icon", ICON_ERROR}});
        safety->disable();
    }

    return {"hyprlooks", "In-process UI system for Hyprland — Vivaldi-styled bars and widgets", "chicken", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    CCrashIsolation::guard("PLUGIN_EXIT", [&]() {
        eventBus()->disconnectAll();
        inputManager()->shutdown();
        widgetRenderer()->shutdown();
        reservation()->clearAll();
        clearWidgetTrees();
        luaAPI()->shutdown();
        Log::logger->log(Log::INFO, "[hyprlooks] shutdown complete");
    });
}
