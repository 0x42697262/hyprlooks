#include "CHyprlandConfig.hpp"
#include "../globals.hpp"

void CHyprlandConfig::notify(const std::string& text, uint64_t timeMs, const CHyprColor& color, eIcons icon) {
    HyprlandAPI::addNotificationV2(PHANDLE, {{"text", "[hyprlooks] " + text}, {"time", timeMs}, {"color", color}, {"icon", icon}});
}

bool CHyprlandConfig::registerDispatcher(const std::string& name, std::function<SDispatchResult(std::string)> handler) {
    return HyprlandAPI::addDispatcherV2(PHANDLE, name, handler);
}

bool CHyprlandConfig::unregisterDispatcher(const std::string& name) {
    return HyprlandAPI::removeDispatcher(PHANDLE, name);
}

bool CHyprlandConfig::addLuaFunction(const std::string& ns, const std::string& name, PLUGIN_LUA_FN fn) {
    return HyprlandAPI::addLuaFunction(PHANDLE, ns, name, fn);
}

bool CHyprlandConfig::removeLuaFunction(const std::string& ns, const std::string& name) {
    return HyprlandAPI::removeLuaFunction(PHANDLE, ns, name);
}
