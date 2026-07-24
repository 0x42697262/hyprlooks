#pragma once

#include <lua.hpp>

#include <unordered_map>
#include <string>
#include <functional>

namespace Hyprlooks {

class CLuaCallbackStore {
  public:
    using Callback = std::function<void()>;

    int  store(lua_State* L, int idx);
    void call(int ref);
    void release(int ref);
    void releaseAll(lua_State* L);

  private:
    struct SStoredCallback {
        int         ref = LUA_NOREF;
        lua_State*  luaState = nullptr;
    };

    std::unordered_map<int, SStoredCallback> m_callbacks;
    int                                       m_nextId = 1;
};

}
