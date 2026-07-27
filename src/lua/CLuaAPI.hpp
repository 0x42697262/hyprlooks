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
        void onPreReload();
        void shutdown();

      private:
        static int luaBar(lua_State* L);
        static int luaPanel(lua_State* L);
        static int luaPopup(lua_State* L);
        static int luaToggle(lua_State* L);
        static int luaShow(lua_State* L);
        static int luaHide(lua_State* L);
        static int luaWidget(lua_State* L);
        static int luaStatus(lua_State* L);
    };

    UP<CLuaAPI>& luaAPI();

}
