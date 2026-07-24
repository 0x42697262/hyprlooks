#include "CNormalState.hpp"

namespace Hyprlooks {

void CNormalState::onEnter(IWidget& w) {
    w.markDirty();
}

void CNormalState::onExit(IWidget& w) {
    ;
}

SWidgetStyle CNormalState::styleFor(const SWidgetStyle& base) {
    return base;
}

const char* CNormalState::name() {
    return "Normal";
}

}
