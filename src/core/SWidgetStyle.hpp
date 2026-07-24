#pragma once

#include <src/helpers/Color.hpp>

#include <hyprutils/math/Vector2D.hpp>

#include <array>
#include <optional>
#include <string>

namespace Hyprlooks {

    struct SBorderRadius {
        float topLeft     = 0.F;
        float topRight    = 0.F;
        float bottomLeft  = 0.F;
        float bottomRight = 0.F;
    };

    struct SBorder {
        float      width         = 0.F;
        CHyprColor color         = CHyprColor{0.F, 0.F, 0.F, 0.F};
        bool       hasGradient   = false;
        CHyprColor gradientColor = CHyprColor{0.F, 0.F, 0.F, 0.F};
    };

    struct SShadow {
        bool       enabled = false;
        float      range   = 20.F;
        CHyprColor color   = CHyprColor{0.F, 0.F, 0.F, 0.4F};
    };

    struct SGlow {
        bool       enabled   = false;
        float      range     = 10.F;
        int        glowPower = 2;
        CHyprColor color     = CHyprColor{0.F, 0.F, 0.F, 0.5F};
    };

    struct SWidgetStyle {
        CHyprColor                bgColor        = CHyprColor{0.F, 0.F, 0.F, 0.F};
        CHyprColor                fgColor        = CHyprColor{1.F, 1.F, 1.F, 1.F};
        CHyprColor                hoverBgColor   = CHyprColor{0.F, 0.F, 0.F, 0.F};
        CHyprColor                hoverFgColor   = CHyprColor{0.F, 0.F, 0.F, 0.F};
        CHyprColor                pressedBgColor = CHyprColor{0.F, 0.F, 0.F, 0.F};
        CHyprColor                pressedFgColor = CHyprColor{0.F, 0.F, 0.F, 0.F};

        SBorderRadius             borderRadius  = {};
        float                     roundingPower = 2.F;

        SBorder                   border = {};
        SShadow                   shadow = {};
        SGlow                     glow   = {};

        bool                      blur      = false;
        float                     blurAlpha = 1.F;

        std::string               fontFamily = "";
        float                     fontSize   = 13.F;
        bool                      fontItalic = false;
        int                       fontWeight = 400;

        Hyprutils::Math::Vector2D padding = {0.F, 0.F};
        Hyprutils::Math::Vector2D margin  = {0.F, 0.F};
        Hyprutils::Math::Vector2D gaps    = {0.F, 0.F};

        float                     opacity = 1.F;

        SWidgetStyle              merge(const SWidgetStyle& override) const;
    };

}
