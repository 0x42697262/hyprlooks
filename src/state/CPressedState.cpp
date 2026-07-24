#include "CPressedState.hpp"

namespace Hyprlooks {

    void CPressedState::onEnter(IWidget& w) {
        w.markDirty();
    }

    void CPressedState::onExit(IWidget& w) {
        w.markDirty();
    }

    SWidgetStyle CPressedState::styleFor(const SWidgetStyle& base) {
        SWidgetStyle s = base;
        if (base.pressedBgColor.a > 0.F)
            s.bgColor = base.pressedBgColor;
        if (base.pressedFgColor.a > 0.F)
            s.fgColor = base.pressedFgColor;
        return s;
    }

    const char* CPressedState::name() {
        return "Pressed";
    }

}
