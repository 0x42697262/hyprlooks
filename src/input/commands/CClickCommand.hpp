#pragma once

#include "../IInputCommand.hpp"
#include "../../core/IWidget.hpp"

#include <hyprutils/math/Vector2D.hpp>
#include <functional>

namespace Hyprlooks {

class CClickCommand : public IInputCommand {
  public:
    using Callback = std::function<void(const Hyprutils::Math::Vector2D&)>;

    CClickCommand(WP<IWidget> widget, uint32_t button, Hyprutils::Math::Vector2D pos, Callback callback);

    virtual void        execute() override;
    virtual const char* name() const override;

  private:
    WP<IWidget>                    m_widget;
    uint32_t                       m_button;
    Hyprutils::Math::Vector2D      m_pos;
    Callback                       m_callback;
};

}
