#include "CHitTester.hpp"

#include <src/helpers/Monitor.hpp>
#include "../widgets/CContainer.hpp"
#include "../widgets/CBar.hpp"

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

        if (w->type() == eWidgetType::BAR) {
            auto*    bar = static_cast<CBar*>(w);
            IWidget* hit = nullptr;
            if (bar->leftSection())
                hit = hitTestRecursive(bar->leftSection(), localPos);
            if (!hit && bar->centerSection())
                hit = hitTestRecursive(bar->centerSection(), localPos);
            if (!hit && bar->rightSection())
                hit = hitTestRecursive(bar->rightSection(), localPos);
            return hit;
        }

        if (w->type() == eWidgetType::CONTAINER) {
            auto* container = static_cast<CContainer*>(w);
            for (const auto& child : container->children()) {
                if (auto* hit = hitTestRecursive(child.get(), localPos))
                    return hit;
            }
        }

        return nullptr;
    }

}
