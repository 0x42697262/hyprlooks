#pragma once

#include "../core/SWidgetStyle.hpp"
#include "../core/IWidget.hpp"

namespace Hyprlooks {

class IWidgetState {
  public:
    virtual ~IWidgetState() = default;

    virtual void         onEnter(IWidget& w) = 0;
    virtual void         onExit(IWidget& w)  = 0;
    virtual SWidgetStyle styleFor(const SWidgetStyle& base) = 0;
    virtual const char*  name() = 0;
};

}
