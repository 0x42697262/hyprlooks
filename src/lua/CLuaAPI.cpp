#include "CLuaAPI.hpp"
#include "CLuaUtils.hpp"
#include "CLuaCallbackStore.hpp"
#include "CLuaWidgetFactory.hpp"

#include "../platform/CHyprlandConfig.hpp"
#include "../platform/CHyprlandReservation.hpp"
#include "../core/CWidgetTree.hpp"
#include "../core/CWidgetRegistry.hpp"
#include "../widgets/CBar.hpp"
#include "../widgets/CContainer.hpp"
#include "../widgets/CPanel.hpp"
#include "../widgets/CLabel.hpp"
#include "../widgets/CButton.hpp"
#include "../widgets/CBox.hpp"
#include "../widgets/CImage.hpp"
#include "../widgets/builtin/CClock.hpp"
#include "../widgets/builtin/CWorkspaceIndicator.hpp"
#include "../layout/CFlexLayout.hpp"
#include "../layout/CStackLayout.hpp"
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

        // gap / gaps: spacing between a container's children (x = main axis).
        // Two balanced blocks (each getField pushes exactly one value that its
        // own lua_pop removes); "gaps" wins if both keys are present.
        if (CLuaUtils::getField(L, "gap", idx)) {
            if (lua_istable(L, -1)) {
                lua_rawgeti(L, -1, 1);
                lua_rawgeti(L, -2, 2);
                style.gaps = {sc<float>(lua_tonumber(L, -2)), sc<float>(lua_tonumber(L, -1))};
                lua_pop(L, 2);
            } else if (lua_isnumber(L, -1)) {
                float g    = sc<float>(lua_tonumber(L, -1));
                style.gaps = {g, g};
            }
        }
        lua_pop(L, 1);

        if (CLuaUtils::getField(L, "gaps", idx)) {
            if (lua_istable(L, -1)) {
                lua_rawgeti(L, -1, 1);
                lua_rawgeti(L, -2, 2);
                style.gaps = {sc<float>(lua_tonumber(L, -2)), sc<float>(lua_tonumber(L, -1))};
                lua_pop(L, 2);
            } else if (lua_isnumber(L, -1)) {
                float g    = sc<float>(lua_tonumber(L, -1));
                style.gaps = {g, g};
            }
        }
        lua_pop(L, 1);

        return style;
    }

    static void parseWidgetList(lua_State* L, int idx, CContainer* container);

    static eFlexDirection parseFlexDirection(const std::string& s) {
        if (s == "column" || s == "vertical" || s == "col")
            return eFlexDirection::COLUMN;
        return eFlexDirection::ROW;
    }

    static eFlexJustify parseFlexJustify(const std::string& s) {
        if (s == "center")
            return eFlexJustify::CENTER;
        if (s == "end")
            return eFlexJustify::END;
        if (s == "space_between" || s == "between")
            return eFlexJustify::SPACE_BETWEEN;
        return eFlexJustify::START;
    }

    static eFlexAlign parseFlexAlign(const std::string& s) {
        if (s == "center")
            return eFlexAlign::CENTER;
        if (s == "end")
            return eFlexAlign::END;
        if (s == "stretch")
            return eFlexAlign::STRETCH;
        return eFlexAlign::START;
    }

    // Reads an optional `layout = { direction, justify, align, gap }` table at
    // table[idx]. Container padding comes from the widget's style, not here, to
    // avoid double-padding. defaultGap keeps flex spacing consistent with the
    // container's style.gaps used during measurement.
    static SFlexOptions parseLayoutOptions(lua_State* L, int idx, float defaultGap) {
        SFlexOptions opts;
        opts.direction = eFlexDirection::ROW;
        opts.justify   = eFlexJustify::START;
        opts.align     = eFlexAlign::CENTER;
        opts.mainGap   = defaultGap;

        idx = lua_absindex(L, idx);
        lua_getfield(L, idx, "layout");
        if (lua_istable(L, -1)) {
            const int t = lua_absindex(L, -1);

            lua_getfield(L, t, "direction");
            if (lua_isstring(L, -1))
                opts.direction = parseFlexDirection(lua_tostring(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, t, "justify");
            if (lua_isstring(L, -1))
                opts.justify = parseFlexJustify(lua_tostring(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, t, "align");
            if (lua_isstring(L, -1))
                opts.align = parseFlexAlign(lua_tostring(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, t, "gap");
            if (lua_isnumber(L, -1))
                opts.mainGap = sc<float>(lua_tonumber(L, -1));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        return opts;
    }

    // Attaches a flex layout (from the optional `layout` table) and parses the
    // `children` array. Shared by container and panel widgets.
    static void applyContainerConfig(lua_State* L, int idx, CContainer* container) {
        if (!container)
            return;

        const float  defaultGap = container->style().gaps.x;
        SFlexOptions opts        = parseLayoutOptions(L, idx, defaultGap);
        container->setLayoutStrategy(makeUnique<CFlexLayout>(opts));

        idx = lua_absindex(L, idx);
        lua_getfield(L, idx, "children");
        if (lua_istable(L, -1))
            parseWidgetList(L, -1, container);
        lua_pop(L, 1);
    }

    static ePanelAnchor parsePanelAnchor(const std::string& s) {
        if (s == "top")
            return ePanelAnchor::TOP;
        if (s == "bottom")
            return ePanelAnchor::BOTTOM;
        if (s == "right")
            return ePanelAnchor::RIGHT;
        if (s == "center")
            return ePanelAnchor::CENTER;
        if (s == "fill")
            return ePanelAnchor::FILL;
        return ePanelAnchor::LEFT;
    }

    static void applyPanelGeometry(lua_State* L, int idx, CPanel* panel) {
        if (!panel)
            return;

        idx = lua_absindex(L, idx);

        lua_getfield(L, idx, "anchor");
        if (lua_isstring(L, -1))
            panel->setAnchor(parsePanelAnchor(lua_tostring(L, -1)));
        lua_pop(L, 1);

        float width  = 0.F;
        float height = 0.F;

        lua_getfield(L, idx, "width");
        if (lua_isnumber(L, -1))
            width = sc<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);

        lua_getfield(L, idx, "height");
        if (lua_isnumber(L, -1))
            height = sc<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);

        panel->setPanelSize(width, height);

        lua_getfield(L, idx, "margin");
        if (lua_istable(L, -1)) {
            lua_rawgeti(L, -1, 1);
            lua_rawgeti(L, -2, 2);
            panel->setMargin({sc<float>(lua_tonumber(L, -2)), sc<float>(lua_tonumber(L, -1))});
            lua_pop(L, 2);
        } else if (lua_isnumber(L, -1)) {
            const float m = sc<float>(lua_tonumber(L, -1));
            panel->setMargin({m, m});
        }
        lua_pop(L, 1);
    }

    static PHLMONITOR resolveMonitor(const std::string& name) {
        if (!g_pCompositor)
            return nullptr;

        PHLMONITOR mon = name.empty() ? g_pCompositor->getMonitorFromCursor() : g_pCompositor->getMonitorFromName(name);
        if (!mon && !g_pCompositor->m_monitors.empty())
            mon = g_pCompositor->m_monitors.front();
        return mon;
    }

    // Each monitor owns one widget tree; its root is a transparent, zero-padding
    // "stage" container with an absolute (stack) layout, so many top-level
    // surfaces (bars, panels, popups) can coexist and overlap by insertion order.
    static CContainer* stageForMonitor(PHLMONITOR mon) {
        auto& tree = widgetTreeForMonitor(mon);
        auto* root = tree->root();
        if (!root || root->type() != eWidgetType::CONTAINER) {
            auto         stage = makeUnique<CContainer>();
            SWidgetStyle transparent;
            transparent.padding = {0.F, 0.F};
            transparent.gaps    = {0.F, 0.F};
            stage->setStyle(transparent);
            stage->setLayoutStrategy(makeUnique<CStackLayout>());
            tree->setRoot(std::move(stage));
            root = tree->root();
        }
        return static_cast<CContainer*>(root);
    }

    static void mountOnMonitor(PHLMONITOR mon, UP<IWidget> widget) {
        if (!mon || !widget)
            return;
        auto* stage = stageForMonitor(mon);
        stage->addChild(std::move(widget));
        widgetTreeForMonitor(mon)->markAllDirty();
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
        } else if (type == "container") {
            applyContainerConfig(L, idx, static_cast<CContainer*>(widget.get()));
        } else if (type == "panel") {
            auto* panel = static_cast<CPanel*>(widget.get());
            applyPanelGeometry(L, idx, panel);
            applyContainerConfig(L, idx, panel);
        } else if (type == "image") {
            auto* img = static_cast<CImage*>(widget.get());

            lua_getfield(L, idx, "path");
            if (lua_isstring(L, -1))
                img->loadFromFile(lua_tostring(L, -1));
            lua_pop(L, 1);

            float iw = 0.F;
            float ih = 0.F;
            lua_getfield(L, idx, "size");
            if (lua_istable(L, -1)) {
                lua_rawgeti(L, -1, 1);
                lua_rawgeti(L, -2, 2);
                iw = sc<float>(lua_tonumber(L, -2));
                ih = sc<float>(lua_tonumber(L, -1));
                lua_pop(L, 2);
            } else if (lua_isnumber(L, -1)) {
                iw = sc<float>(lua_tonumber(L, -1));
                ih = iw;
            }
            lua_pop(L, 1);

            if (iw > 0.F || ih > 0.F)
                img->setSize({iw, ih});
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

        PHLMONITOR targetMonitor = resolveMonitor(bar->monitorName());
        if (!targetMonitor) {
            Log::logger->log(Log::WARN, "[hyprlooks] no monitor found for bar");
            return 0;
        }

        // Reserve monitor space (default on for bars) so windows do not overlap.
        bool doReserve = true;
        lua_getfield(L, 1, "reserve");
        if (lua_isboolean(L, -1))
            doReserve = lua_toboolean(L, -1);
        lua_pop(L, 1);

        if (doReserve) {
            const double monW      = targetMonitor->m_size.x;
            const double edgeWidth = bar->width() > 0.F ? bar->width() : monW * 0.1;
            switch (bar->position()) {
                case eBarPosition::BOTTOM: reservation()->reserveEdge(targetMonitor, eReserveEdge::BOTTOM, bar->height()); break;
                case eBarPosition::LEFT: reservation()->reserveEdge(targetMonitor, eReserveEdge::LEFT, edgeWidth); break;
                case eBarPosition::RIGHT: reservation()->reserveEdge(targetMonitor, eReserveEdge::RIGHT, edgeWidth); break;
                default: reservation()->reserveEdge(targetMonitor, eReserveEdge::TOP, bar->height()); break;
            }
        }

        mountOnMonitor(targetMonitor, std::move(bar));
        return 0;
    }

    int CLuaAPI::luaPanel(lua_State* L) {
        luaL_checktype(L, 1, LUA_TTABLE);

        auto  panel = makeUnique<CPanel>();
        auto* raw   = panel.get();

        lua_getfield(L, 1, "style");
        if (lua_istable(L, -1)) {
            auto style = parseStyleFromLua(L, -1);
            raw->setStyle(theme()->resolve(eWidgetType::PANEL, style));
        }
        lua_pop(L, 1);

        applyPanelGeometry(L, 1, raw);
        applyContainerConfig(L, 1, raw);

        std::string monitorName;
        lua_getfield(L, 1, "monitor");
        if (lua_isstring(L, -1))
            monitorName = lua_tostring(L, -1);
        lua_pop(L, 1);

        PHLMONITOR targetMonitor = resolveMonitor(monitorName);
        if (!targetMonitor) {
            Log::logger->log(Log::WARN, "[hyprlooks] no monitor found for panel");
            return 0;
        }

        // Panels do not reserve space unless asked to (many are floating docks).
        bool doReserve = false;
        lua_getfield(L, 1, "reserve");
        if (lua_isboolean(L, -1))
            doReserve = lua_toboolean(L, -1);
        lua_pop(L, 1);

        if (doReserve) {
            const double mx = raw->margin().x;
            const double my = raw->margin().y;
            const double w  = raw->panelWidth() > 0.F ? raw->panelWidth() : 64.0;
            const double h  = raw->panelHeight() > 0.F ? raw->panelHeight() : 64.0;
            switch (raw->anchor()) {
                case ePanelAnchor::LEFT: reservation()->reserveEdge(targetMonitor, eReserveEdge::LEFT, mx + w); break;
                case ePanelAnchor::RIGHT: reservation()->reserveEdge(targetMonitor, eReserveEdge::RIGHT, mx + w); break;
                case ePanelAnchor::TOP: reservation()->reserveEdge(targetMonitor, eReserveEdge::TOP, my + h); break;
                case ePanelAnchor::BOTTOM: reservation()->reserveEdge(targetMonitor, eReserveEdge::BOTTOM, my + h); break;
                default: break; // CENTER / FILL do not reserve
            }
        }

        mountOnMonitor(targetMonitor, std::move(panel));
        return 0;
    }

    int CLuaAPI::luaWidget(lua_State* L) {
        luaL_checktype(L, 1, LUA_TTABLE);

        auto widget = parseWidgetFromLua(L, 1);
        if (!widget)
            return 0;

        std::string monitorName;
        lua_getfield(L, 1, "monitor");
        if (lua_isstring(L, -1))
            monitorName = lua_tostring(L, -1);
        lua_pop(L, 1);

        PHLMONITOR targetMonitor = resolveMonitor(monitorName);
        if (!targetMonitor) {
            Log::logger->log(Log::WARN, "[hyprlooks] no monitor found for widget");
            return 0;
        }

        mountOnMonitor(targetMonitor, std::move(widget));
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
        if (!config.addLuaFunction("hyprlooks", "panel", &luaPanel)) {
            Log::logger->log(Log::ERR, "[hyprlooks] failed to register lua function: panel");
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
