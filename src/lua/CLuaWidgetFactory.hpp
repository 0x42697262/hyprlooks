#pragma once

#include <src/defines.hpp>

#include <lua.hpp>

#include <string>

namespace Hyprlooks {

class CLuaWidgetFactory {
  public:
    CLuaWidgetFactory()  = default;
    ~CLuaWidgetFactory() = default;

    void registerBuiltinWidgets();

  private:
    void registerWidgetTypes();
};

UP<CLuaWidgetFactory>& luaWidgetFactory();

}
