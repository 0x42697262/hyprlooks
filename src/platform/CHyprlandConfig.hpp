#pragma once

#include <src/plugins/PluginAPI.hpp>

#include <hyprutils/memory/SharedPtr.hpp>

#include <string>
#include <functional>

namespace Config {
    class CConfigManager;
}

class CHyprlandConfig {
  public:
    CHyprlandConfig()  = default;
    ~CHyprlandConfig() = default;

    void notify(const std::string& text, uint64_t timeMs = 10000, const CHyprColor& color = CHyprColor{0}, eIcons icon = ICON_WARNING);
    bool registerDispatcher(const std::string& name, std::function<SDispatchResult(std::string)> handler);
    bool unregisterDispatcher(const std::string& name);
    bool addLuaFunction(const std::string& ns, const std::string& name, PLUGIN_LUA_FN fn);
    bool removeLuaFunction(const std::string& ns, const std::string& name);
};
