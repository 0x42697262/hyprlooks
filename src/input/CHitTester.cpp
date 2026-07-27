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

        if (w->type() == eWidgetType::CONTAINER || w->type() == eWidgetType::PANEL || w->type() == eWidgetType::SCROLL) {
            auto*       container = static_cast<CContainer*>(w);
            const auto& children  = container->children();
            // iterate topmost-first (children draw in order, so last == on top)
            // so overlapping surfaces (e.g. a popup over a bar) hit correctly.
            for (auto it = children.rbegin(); it != children.rend(); ++it) {
                if (auto* hit = hitTestRecursive(it->get(), localPos))
                    return hit;
            }
        }

        return nullptr;
    }

    IWidget* CHitTester::scrollableAt(CWidgetTree* tree, const Hyprutils::Math::Vector2D& globalPos) {
        if (!tree || !tree->root() || !tree->monitor())
            return nullptr;

        const auto monitorPos = tree->monitor()->m_position;
        const auto localPos   = Hyprutils::Math::Vector2D{globalPos.x - monitorPos.x, globalPos.y - monitorPos.y};

        return scrollableRecursive(tree->root(), localPos);
    }

    IWidget* CHitTester::scrollableRecursive(IWidget* w, const Hyprutils::Math::Vector2D& localPos) {
        if (!w || !w->isVisible() || !w->geometry().containsPoint(localPos))
            return nullptr;

        IWidget* found = w->type() == eWidgetType::SCROLL ? w : nullptr;

        if (w->type() == eWidgetType::CONTAINER || w->type() == eWidgetType::PANEL || w->type() == eWidgetType::SCROLL) {
            auto* container = static_cast<CContainer*>(w);
            for (const auto& child : container->children()) {
                if (auto* deeper = scrollableRecursive(child.get(), localPos))
                    found = deeper; // deepest scroll under the cursor wins
            }
        }

        return found;
    }

}
