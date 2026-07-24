#pragma once

#include "../IInputCommand.hpp"
#include "../../core/IWidget.hpp"

#include <hyprutils/math/Vector2D.hpp>
#include <functional>

namespace Hyprlooks {

class CHoverCommand : public IInputCommand {
  public:
    using Callback = std::function<void(const Hyprutils::Math::Vector2D&)>;

    CHoverCommand(WP<IWidget> widget, Hyprutils::Math::Vector2D pos, Callback callback);

    virtual void        execute() override;
    virtual const char* name() const override;

  private:
    WP<IWidget>                    m_widget;
    Hyprutils::Math::Vector2D      m_pos;
    Callback                       m_callback;
};

}
