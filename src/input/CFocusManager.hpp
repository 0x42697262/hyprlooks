#pragma once

#include "../core/IWidget.hpp"

#include <hyprutils/memory/WeakPtr.hpp>

namespace Hyprlooks {

class CFocusManager {
  public:
    CFocusManager()  = default;
    ~CFocusManager() = default;

    void        setFocused(WP<IWidget> widget);
    WP<IWidget> focused() const;
    void        clearFocus();
    bool        hasFocus() const;

  private:
    WP<IWidget> m_focused;
};

}
