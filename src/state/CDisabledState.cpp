#include "CDisabledState.hpp"

namespace Hyprlooks {

    void CDisabledState::onEnter(IWidget& w) {
        w.markDirty();
    }

    void CDisabledState::onExit(IWidget& w) {
        w.markDirty();
    }

    SWidgetStyle CDisabledState::styleFor(const SWidgetStyle& base) {
        SWidgetStyle s = base;
        s.opacity *= 0.5F;
        return s;
    }

    const char* CDisabledState::name() {
        return "Disabled";
    }

}
