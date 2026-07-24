#pragma once

#include "../core/SWidgetStyle.hpp"

#include <string>

namespace Hyprlooks {

class CThemeDefaults {
  public:
    static SWidgetStyle barStyle();
    static SWidgetStyle containerStyle();
    static SWidgetStyle labelStyle();
    static SWidgetStyle boxStyle();
    static SWidgetStyle buttonStyle();
    static SWidgetStyle buttonHoverStyle();
    static SWidgetStyle buttonPressedStyle();
    static SWidgetStyle imageStyle();

    static CHyprColor accentColor();
    static CHyprColor bgColor();
    static CHyprColor surfaceColor();
    static CHyprColor textColor();
    static CHyprColor mutedTextColor();
};

}
