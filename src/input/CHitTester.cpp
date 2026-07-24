#include "CHitTester.hpp"

#include <src/helpers/Monitor.hpp>

namespace Hyprlooks {

IWidget* CHitTester::hitTest(CWidgetTree* tree, const Hyprutils::Math::Vector2D& globalPos) {
    if (!tree || !tree->root() || !tree->monitor())
        return nullptr;

    const auto monitorPos = tree->monitor()->m_position;
    const auto localPos   = Hyprutils::Math::Vector2D{globalPos.x - monitorPos.x, globalPos.y - monitorPos.y};

    return hitTestRecursive(tree->root(), localPos);
}

IWidget* CHitTester::hitTestRecursive(IWidget* w, const Hyprutils::Math::Vector2D& localPos) {
    if (!w || !w->isVisible())
        return nullptr;

    if (!w->geometry().containsPoint(localPos))
        return nullptr;

    if (w->interactive())
        return w;

    return nullptr;
}

}
