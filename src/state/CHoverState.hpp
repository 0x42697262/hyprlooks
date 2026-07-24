#pragma once

#include "IWidgetState.hpp"

namespace Hyprlooks {

class CHoverState : public IWidgetState {
  public:
    virtual void         onEnter(IWidget& w) override;
    virtual void         onExit(IWidget& w) override;
    virtual SWidgetStyle styleFor(const SWidgetStyle& base) override;
    virtual const char*  name() override;
};

}
