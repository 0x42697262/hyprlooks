#include "CLuaCallbackStore.hpp"

#include <src/debug/log/Logger.hpp>

namespace Hyprlooks {

    int CLuaCallbackStore::store(lua_State* L, int idx) {
        if (!lua_isfunction(L, idx))
            return LUA_NOREF;

        lua_pushvalue(L, idx);
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);

        int id          = m_nextId++;
        m_callbacks[id] = SStoredCallback{ref, L};
        return id;
    }

    void CLuaCallbackStore::call(int id) {
        auto it = m_callbacks.find(id);
        if (it == m_callbacks.end())
            return;

        const auto& cb = it->second;
        if (!cb.luaState || cb.ref == LUA_NOREF)
            return;

        lua_rawgeti(cb.luaState, LUA_REGISTRYINDEX, cb.ref);
        if (lua_pcall(cb.luaState, 0, 0, 0) != LUA_OK) {
            const char* err = lua_tostring(cb.luaState, -1);
            Log::logger->log(Log::ERR, "[hyprlooks] Lua callback error: {}", err ? err : "unknown");
            lua_pop(cb.luaState, 1);
        }
    }

    void CLuaCallbackStore::release(int id) {
        auto it = m_callbacks.find(id);
        if (it == m_callbacks.end())
            return;

        if (it->second.luaState && it->second.ref != LUA_NOREF)
            luaL_unref(it->second.luaState, LUA_REGISTRYINDEX, it->second.ref);
        m_callbacks.erase(it);
    }

    void CLuaCallbackStore::releaseAll() {
        for (auto& [id, cb] : m_callbacks) {
            if (cb.luaState && cb.ref != LUA_NOREF)
                luaL_unref(cb.luaState, LUA_REGISTRYINDEX, cb.ref);
        }
        m_callbacks.clear();
    }

}
