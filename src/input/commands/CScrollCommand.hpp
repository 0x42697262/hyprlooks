#pragma once

#include "../IInputCommand.hpp"
#include "../../core/IWidget.hpp"

#include <functional>

namespace Hyprlooks {

class CScrollCommand : public IInputCommand {
  public:
    using Callback = std::function<void(int direction, double delta)>;

    CScrollCommand(WP<IWidget> widget, int direction, double delta, Callback callback);

    virtual void        execute() override;
    virtual const char* name() const override;

  private:
    WP<IWidget> m_widget;
    int         m_direction;
    double      m_delta;
    Callback    m_callback;
};

}
