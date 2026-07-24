#pragma once

#include <src/helpers/Color.hpp>
#include <lua.hpp>

#include <string>

namespace Hyprlooks {

class CLuaUtils {
  public:
    static bool          getField(lua_State* L, const char* key, int stackIdx = -1);
    static bool          isString(lua_State* L, int idx);
    static bool          isNumber(lua_State* L, int idx);
    static bool          isBool(lua_State* L, int idx);
    static bool          isTable(lua_State* L, int idx);
    static bool          isFunction(lua_State* L, int idx);

    static std::string   optString(lua_State* L, int idx, const std::string& def);
    static double        optNumber(lua_State* L, int idx, double def);
    static bool          optBool(lua_State* L, int idx, bool def);

    static int           newRegistryRef(lua_State* L, int idx);
    static void          releaseRef(lua_State* L, int ref);

    static CHyprColor    optColor(lua_State* L, int idx, const CHyprColor& def);
    static CHyprColor    colorFromTable(lua_State* L, int idx);
};

}
