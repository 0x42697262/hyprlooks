#pragma once

#include <src/defines.hpp>

#include <lua.hpp>

#include <string>

namespace Hyprlooks {

class CLuaAPI {
  public:
    CLuaAPI()  = default;
    ~CLuaAPI() = default;

    bool init();
    void shutdown();

  private:
    static int luaBar(lua_State* L);
    static int luaWidget(lua_State* L);
    static int luaStatus(lua_State* L);
};

UP<CLuaAPI>& luaAPI();

}
