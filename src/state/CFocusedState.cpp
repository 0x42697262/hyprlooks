#include "CFocusedState.hpp"

namespace Hyprlooks {

void CFocusedState::onEnter(IWidget& w) {
    w.markDirty();
}

void CFocusedState::onExit(IWidget& w) {
    w.markDirty();
}

SWidgetStyle CFocusedState::styleFor(const SWidgetStyle& base) {
    return base;
}

const char* CFocusedState::name() {
    return "Focused";
}

}
