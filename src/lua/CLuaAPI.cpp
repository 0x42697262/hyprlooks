#include "CLuaAPI.hpp"
#include "CLuaUtils.hpp"
#include "CLuaCallbackStore.hpp"
#include "CLuaWidgetFactory.hpp"

#include "../platform/CHyprlandConfig.hpp"
#include "../core/CWidgetTree.hpp"
#include "../core/CWidgetRegistry.hpp"
#include "../widgets/CBar.hpp"
#include "../widgets/CContainer.hpp"
#include "../widgets/CLabel.hpp"
#include "../widgets/CButton.hpp"
#include "../widgets/CBox.hpp"
#include "../widgets/builtin/CClock.hpp"
#include "../widgets/builtin/CWorkspaceIndicator.hpp"
#include "../layout/CFlexLayout.hpp"
#include "../theme/CTheme.hpp"
#include "../safety/CSafetyManager.hpp"

#include "../globals.hpp"

#include <src/Compositor.hpp>
#include <src/debug/log/Logger.hpp>

namespace Hyprlooks {

    static UP<CLuaCallbackStore>& callbackStore() {
        static UP<CLuaCallbackStore> store = makeUnique<CLuaCallbackStore>();
        return store;
    }

    static SWidgetStyle parseStyleFromLua(lua_State* L, int idx) {
        SWidgetStyle style;
        if (!lua_istable(L, idx))
            return style;

        idx = lua_absindex(L, idx);

        if (CLuaUtils::getField(L, "bg", idx)) {
            style.bgColor = CLuaUtils::colorFromTable(L, -1);
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "fg", idx)) {
            style.fgColor = CLuaUtils::colorFromTable(L, -1);
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "hover_bg", idx)) {
            style.hoverBgColor = CLuaUtils::colorFromTable(L, -1);
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "blur", idx)) {
            style.blur = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "blur_alpha", idx)) {
            style.blurAlpha = sc<float>(lua_tonumber(L, -1));
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "font_size", idx)) {
            style.fontSize = sc<float>(lua_tonumber(L, -1));
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "font_family", idx)) {
            style.fontFamily = lua_tostring(L, -1);
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "padding", idx)) {
            if (lua_istable(L, -1)) {
                lua_rawgeti(L, -1, 1);
                lua_rawgeti(L, -2, 2);
                style.padding = {sc<float>(lua_tonumber(L, -2)), sc<float>(lua_tonumber(L, -1))};
                lua_pop(L, 2);
            } else if (lua_isnumber(L, -1)) {
                style.padding = {sc<float>(lua_tonumber(L, -1)), sc<float>(lua_tonumber(L, -1))};
            }
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "border_radius", idx)) {
            if (lua_istable(L, -1)) {
                lua_rawgeti(L, -1, 1);
                lua_rawgeti(L, -2, 2);
                lua_rawgeti(L, -3, 3);
                lua_rawgeti(L, -4, 4);
                style.borderRadius.topLeft     = sc<float>(lua_tonumber(L, -4));
                style.borderRadius.topRight    = sc<float>(lua_tonumber(L, -3));
                style.borderRadius.bottomLeft  = sc<float>(lua_tonumber(L, -2));
                style.borderRadius.bottomRight = sc<float>(lua_tonumber(L, -1));
                lua_pop(L, 4);
            } else if (lua_isnumber(L, -1)) {
                float r            = sc<float>(lua_tonumber(L, -1));
                style.borderRadius = {r, r, r, r};
            }
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "opacity", idx)) {
            style.opacity = sc<float>(lua_tonumber(L, -1));
        }
        lua_pop(L, 1);

        return style;
    }

    static UP<IWidget> parseWidgetFromLua(lua_State* L, int idx) {
        if (!lua_istable(L, idx))
            return nullptr;

        idx = lua_absindex(L, idx);

        lua_getfield(L, idx, "type");
        std::string type = lua_isstring(L, -1) ? lua_tostring(L, -1) : "";
        lua_pop(L, 1);

        if (type.empty())
            return nullptr;

        auto widget = widgetRegistry()->create(type);
        if (!widget) {
            Log::logger->log(Log::WARN, "[hyprlooks] unknown widget type: {}", type);
            return nullptr;
        }

        lua_getfield(L, idx, "style");
        if (lua_istable(L, -1)) {
            auto style = parseStyleFromLua(L, -1);
            widget->setStyle(theme()->resolve(widget->type(), style));
        }
        lua_pop(L, 1);

        if (type == "label") {
            lua_getfield(L, idx, "text");
            if (lua_isstring(L, -1)) {
                auto* label = static_cast<CLabel*>(widget.get());
                label->setText(lua_tostring(L, -1));
            }
            lua_pop(L, 1);
        } else if (type == "button") {
            lua_getfield(L, idx, "text");
            if (lua_isstring(L, -1)) {
                auto* btn = static_cast<CButton*>(widget.get());
                btn->setText(lua_tostring(L, -1));
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "icon");
            if (lua_isstring(L, -1)) {
                auto* btn = static_cast<CButton*>(widget.get());
                btn->setIcon(lua_tostring(L, -1));
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "on_click");
            if (lua_isfunction(L, -1)) {
                int   cbId = callbackStore()->store(L, -1);
                auto* btn  = static_cast<CButton*>(widget.get());
                btn->onClick([cbId]() { callbackStore()->call(cbId); });
            }
            lua_pop(L, 1);
        } else if (type == "clock") {
            lua_getfield(L, idx, "format");
            if (lua_isstring(L, -1)) {
                auto* clock = static_cast<CClock*>(widget.get());
                clock->setFormat(lua_tostring(L, -1));
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "interval");
            if (lua_isnumber(L, -1)) {
                auto* clock = static_cast<CClock*>(widget.get());
                clock->setIntervalMs(sc<uint32_t>(lua_tonumber(L, -1)));
            }
            lua_pop(L, 1);
        } else if (type == "workspaces") {
            auto* wi = static_cast<CWorkspaceIndicator*>(widget.get());

            lua_getfield(L, idx, "monitor");
            if (lua_isstring(L, -1))
                wi->setMonitorName(lua_tostring(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, idx, "style");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "active_bg");
                if (lua_istable(L, -1))
                    wi->setActiveBgColor(CLuaUtils::colorFromTable(L, -1));
                lua_pop(L, 1);

                lua_getfield(L, -1, "inactive_fg");
                if (lua_istable(L, -1))
                    wi->setInactiveFgColor(CLuaUtils::colorFromTable(L, -1));
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }

        return widget;
    }

    static void parseWidgetList(lua_State* L, int idx, CContainer* container) {
        if (!lua_istable(L, idx))
            return;

        idx = lua_absindex(L, idx);
        lua_pushnil(L);
        while (lua_next(L, idx) != 0) {
            auto widget = parseWidgetFromLua(L, -1);
            if (widget)
                container->addChild(std::move(widget));
            lua_pop(L, 1);
        }
    }

    int CLuaAPI::luaBar(lua_State* L) {
        luaL_checktype(L, 1, LUA_TTABLE);

        auto bar = makeUnique<CBar>();

        lua_getfield(L, 1, "position");
        if (lua_isstring(L, -1)) {
            std::string pos = lua_tostring(L, -1);
            if (pos == "bottom")
                bar->setPosition(eBarPosition::BOTTOM);
            else if (pos == "left")
                bar->setPosition(eBarPosition::LEFT);
            else if (pos == "right")
                bar->setPosition(eBarPosition::RIGHT);
        }
        lua_pop(L, 1);

        lua_getfield(L, 1, "height");
        if (lua_isnumber(L, -1))
            bar->setHeight(sc<float>(lua_tonumber(L, -1)));
        lua_pop(L, 1);

        lua_getfield(L, 1, "monitor");
        if (lua_isstring(L, -1))
            bar->setMonitorName(lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_getfield(L, 1, "style");
        if (lua_istable(L, -1)) {
            auto style = parseStyleFromLua(L, -1);
            bar->setStyle(theme()->resolve(eWidgetType::BAR, style));
        }
        lua_pop(L, 1);

        lua_getfield(L, 1, "left");
        if (lua_istable(L, -1))
            parseWidgetList(L, -1, bar->leftSection());
        lua_pop(L, 1);

        lua_getfield(L, 1, "center");
        if (lua_istable(L, -1))
            parseWidgetList(L, -1, bar->centerSection());
        lua_pop(L, 1);

        lua_getfield(L, 1, "right");
        if (lua_istable(L, -1))
            parseWidgetList(L, -1, bar->rightSection());
        lua_pop(L, 1);

        auto       monitorName   = bar->monitorName();
        PHLMONITOR targetMonitor = nullptr;

        if (monitorName.empty()) {
            targetMonitor = g_pCompositor ? g_pCompositor->getMonitorFromCursor() : nullptr;
        } else {
            if (g_pCompositor)
                targetMonitor = g_pCompositor->getMonitorFromName(monitorName);
        }

        if (!targetMonitor && g_pCompositor && !g_pCompositor->m_monitors.empty())
            targetMonitor = g_pCompositor->m_monitors.front();

        if (targetMonitor) {
            auto& tree = widgetTreeForMonitor(targetMonitor);
            tree->setRoot(std::move(bar));
            tree->markAllDirty();
        } else {
            Log::logger->log(Log::WARN, "[hyprlooks] no monitor found for bar");
        }

        return 0;
    }

    int CLuaAPI::luaWidget(lua_State* L) {
        luaL_checktype(L, 1, LUA_TTABLE);
        auto widget = parseWidgetFromLua(L, 1);
        if (!widget)
            return 0;

        return 0;
    }

    int CLuaAPI::luaStatus(lua_State* L) {
        auto& safety = safetyManager();
        lua_pushstring(L, safety->statusString().c_str());
        return 1;
    }

    bool CLuaAPI::init() {
        CHyprlandConfig config;

        if (!config.addLuaFunction("hyprlooks", "bar", &luaBar)) {
            Log::logger->log(Log::ERR, "[hyprlooks] failed to register lua function: bar");
            return false;
        }
        if (!config.addLuaFunction("hyprlooks", "widget", &luaWidget)) {
            Log::logger->log(Log::ERR, "[hyprlooks] failed to register lua function: widget");
            return false;
        }
        if (!config.addLuaFunction("hyprlooks", "status", &luaStatus)) {
            Log::logger->log(Log::ERR, "[hyprlooks] failed to register lua function: status");
            return false;
        }

        luaWidgetFactory()->registerBuiltinWidgets();
        theme()->loadDefaults();

        return true;
    }

    void CLuaAPI::shutdown() {
        callbackStore()->releaseAll();
    }

    void CLuaAPI::onPreReload() {
        callbackStore()->releaseAll();
    }

    UP<CLuaAPI>& luaAPI() {
        static UP<CLuaAPI> api = makeUnique<CLuaAPI>();
        return api;
    }

}
