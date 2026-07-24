#include "SWidgetStyle.hpp"

namespace Hyprlooks {

    SWidgetStyle SWidgetStyle::merge(const SWidgetStyle& o) const {
        SWidgetStyle r = *this;

        if (o.bgColor.a > 0.F)
            r.bgColor = o.bgColor;
        if (o.fgColor.a > 0.F)
            r.fgColor = o.fgColor;
        if (o.hoverBgColor.a > 0.F)
            r.hoverBgColor = o.hoverBgColor;
        if (o.hoverFgColor.a > 0.F)
            r.hoverFgColor = o.hoverFgColor;
        if (o.pressedBgColor.a > 0.F)
            r.pressedBgColor = o.pressedBgColor;
        if (o.pressedFgColor.a > 0.F)
            r.pressedFgColor = o.pressedFgColor;

        if (o.borderRadius.topLeft > 0.F || o.borderRadius.topRight > 0.F || o.borderRadius.bottomLeft > 0.F || o.borderRadius.bottomRight > 0.F)
            r.borderRadius = o.borderRadius;
        if (o.roundingPower != 2.F)
            r.roundingPower = o.roundingPower;

        if (o.border.width > 0.F)
            r.border = o.border;
        if (o.shadow.enabled)
            r.shadow = o.shadow;
        if (o.glow.enabled)
            r.glow = o.glow;

        if (o.blur)
            r.blur = true;
        if (o.blurAlpha != 1.F)
            r.blurAlpha = o.blurAlpha;

        if (!o.fontFamily.empty())
            r.fontFamily = o.fontFamily;
        if (o.fontSize != 13.F)
            r.fontSize = o.fontSize;
        if (o.fontItalic)
            r.fontItalic = true;
        if (o.fontWeight != 400)
            r.fontWeight = o.fontWeight;

        if (o.padding.x > 0.F || o.padding.y > 0.F)
            r.padding = o.padding;
        if (o.margin.x > 0.F || o.margin.y > 0.F)
            r.margin = o.margin;
        if (o.gaps.x > 0.F || o.gaps.y > 0.F)
            r.gaps = o.gaps;

        if (o.opacity != 1.F)
            r.opacity = o.opacity;

        return r;
    }

}
