#include "CLuaUtils.hpp"

namespace Hyprlooks {

    bool CLuaUtils::getField(lua_State* L, const char* key, int stackIdx) {
        lua_pushvalue(L, stackIdx);
        lua_getfield(L, -1, key);
        lua_replace(L, -2);
        return !lua_isnil(L, -1);
    }

    bool CLuaUtils::isString(lua_State* L, int idx) {
        return lua_type(L, idx) == LUA_TSTRING;
    }

    bool CLuaUtils::isNumber(lua_State* L, int idx) {
        return lua_type(L, idx) == LUA_TNUMBER;
    }

    bool CLuaUtils::isBool(lua_State* L, int idx) {
        return lua_type(L, idx) == LUA_TBOOLEAN;
    }

    bool CLuaUtils::isTable(lua_State* L, int idx) {
        return lua_type(L, idx) == LUA_TTABLE;
    }

    bool CLuaUtils::isFunction(lua_State* L, int idx) {
        return lua_type(L, idx) == LUA_TFUNCTION;
    }

    std::string CLuaUtils::optString(lua_State* L, int idx, const std::string& def) {
        if (lua_isstring(L, idx))
            return lua_tostring(L, idx);
        return def;
    }

    double CLuaUtils::optNumber(lua_State* L, int idx, double def) {
        if (lua_isnumber(L, idx))
            return lua_tonumber(L, idx);
        return def;
    }

    bool CLuaUtils::optBool(lua_State* L, int idx, bool def) {
        if (lua_isboolean(L, idx))
            return lua_toboolean(L, idx);
        return def;
    }

    int CLuaUtils::newRegistryRef(lua_State* L, int idx) {
        lua_pushvalue(L, idx);
        return luaL_ref(L, LUA_REGISTRYINDEX);
    }

    void CLuaUtils::releaseRef(lua_State* L, int ref) {
        if (ref != LUA_NOREF && ref != LUA_REFNIL)
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }

    CHyprColor CLuaUtils::optColor(lua_State* L, int idx, const CHyprColor& def) {
        if (!lua_istable(L, idx))
            return def;
        return colorFromTable(L, idx);
    }

    CHyprColor CLuaUtils::colorFromTable(lua_State* L, int idx) {
        CHyprColor c;
        if (!lua_istable(L, idx))
            return c;

        // Normalize to an absolute index: the lua_rawgeti calls below push values
        // onto the stack, which would shift a relative (negative) idx out from under
        // the table and cause a raw access on a non-table value.
        idx = lua_absindex(L, idx);

        lua_rawgeti(L, idx, 1);
        lua_rawgeti(L, idx, 2);
        lua_rawgeti(L, idx, 3);
        lua_rawgeti(L, idx, 4);

        c.r = sc<float>(lua_tonumber(L, -4));
        c.g = sc<float>(lua_tonumber(L, -3));
        c.b = sc<float>(lua_tonumber(L, -2));
        c.a = sc<float>(luaL_optnumber(L, -1, 1.0));

        lua_pop(L, 4);
        return c;
    }

}
