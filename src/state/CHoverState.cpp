#include "CHoverState.hpp"

namespace Hyprlooks {

    void CHoverState::onEnter(IWidget& w) {
        w.markDirty();
    }

    void CHoverState::onExit(IWidget& w) {
        w.markDirty();
    }

    SWidgetStyle CHoverState::styleFor(const SWidgetStyle& base) {
        SWidgetStyle s = base;
        if (base.hoverBgColor.a > 0.F)
            s.bgColor = base.hoverBgColor;
        if (base.hoverFgColor.a > 0.F)
            s.fgColor = base.hoverFgColor;
        return s;
    }

    const char* CHoverState::name() {
        return "Hover";
    }

}
