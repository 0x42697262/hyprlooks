#include "CThemeDefaults.hpp"

namespace Hyprlooks {

CHyprColor CThemeDefaults::accentColor() {
    return CHyprColor{0.2F, 0.5F, 0.9F, 1.F};
}

CHyprColor CThemeDefaults::bgColor() {
    return CHyprColor{0.08F, 0.08F, 0.10F, 0.85F};
}

CHyprColor CThemeDefaults::surfaceColor() {
    return CHyprColor{0.12F, 0.12F, 0.16F, 1.F};
}

CHyprColor CThemeDefaults::textColor() {
    return CHyprColor{0.9F, 0.9F, 0.95F, 1.F};
}

CHyprColor CThemeDefaults::mutedTextColor() {
    return CHyprColor{0.5F, 0.5F, 0.55F, 1.F};
}

SWidgetStyle CThemeDefaults::barStyle() {
    SWidgetStyle s;
    s.bgColor       = bgColor();
    s.blur          = true;
    s.blurAlpha     = 0.8F;
    s.border.width  = 1.F;
    s.border.color  = CHyprColor{0.15F, 0.15F, 0.20F, 0.8F};
    s.shadow.enabled = true;
    s.shadow.range   = 20.F;
    s.shadow.color   = CHyprColor{0.F, 0.F, 0.F, 0.4F};
    return s;
}

SWidgetStyle CThemeDefaults::containerStyle() {
    SWidgetStyle s;
    s.padding = {8.F, 4.F};
    s.gaps    = {4.F, 4.F};
    return s;
}

SWidgetStyle CThemeDefaults::labelStyle() {
    SWidgetStyle s;
    s.fgColor   = textColor();
    s.fontSize  = 13.F;
    return s;
}

SWidgetStyle CThemeDefaults::boxStyle() {
    SWidgetStyle s;
    s.bgColor      = surfaceColor();
    s.borderRadius = {8.F, 8.F, 8.F, 8.F};
    return s;
}

SWidgetStyle CThemeDefaults::buttonStyle() {
    SWidgetStyle s;
    s.bgColor       = surfaceColor();
    s.fgColor       = textColor();
    s.hoverBgColor  = CHyprColor{0.15F, 0.15F, 0.20F, 1.F};
    s.pressedBgColor = CHyprColor{0.1F, 0.1F, 0.14F, 1.F};
    s.borderRadius  = {6.F, 6.F, 6.F, 6.F};
    s.padding       = {8.F, 4.F};
    s.fontSize      = 13.F;
    return s;
}

SWidgetStyle CThemeDefaults::buttonHoverStyle() {
    SWidgetStyle s;
    s.hoverBgColor = CHyprColor{0.15F, 0.15F, 0.20F, 1.F};
    return s;
}

SWidgetStyle CThemeDefaults::buttonPressedStyle() {
    SWidgetStyle s;
    s.pressedBgColor = CHyprColor{0.1F, 0.1F, 0.14F, 1.F};
    return s;
}

SWidgetStyle CThemeDefaults::imageStyle() {
    SWidgetStyle s;
    return s;
}

}
