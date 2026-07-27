#pragma once

#include "../core/IWidget.hpp"
#include "../core/CWidgetTree.hpp"

#include <hyprutils/math/Vector2D.hpp>
#include <hyprutils/math/Box.hpp>

#include <vector>

namespace Hyprlooks {

    class CHitTester {
      public:
        CHitTester()  = default;
        ~CHitTester() = default;

        IWidget* hitTest(CWidgetTree* tree, const Hyprutils::Math::Vector2D& globalPos);

        // Deepest scrollable container under the cursor, or null. Used to route
        // mouse-wheel events regardless of which (interactive) child is hit.
        IWidget* scrollableAt(CWidgetTree* tree, const Hyprutils::Math::Vector2D& globalPos);

      private:
        IWidget* hitTestRecursive(IWidget* w, const Hyprutils::Math::Vector2D& localPos);
        IWidget* scrollableRecursive(IWidget* w, const Hyprutils::Math::Vector2D& localPos);
    };

}
