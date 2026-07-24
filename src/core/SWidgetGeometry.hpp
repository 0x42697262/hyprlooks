#pragma once

#include <hyprutils/math/Box.hpp>
#include <hyprutils/math/Vector2D.hpp>

namespace Hyprlooks {

    struct SWidgetGeometry {
        Hyprutils::Math::CBox box;     // monitor-local logical coordinates
        Hyprutils::Math::CBox clipBox; // clipping region (for overflow/scroll)

        bool                  visible = false;
        bool                  dirty   = true;

        void                  reset();
        Hyprutils::Math::CBox globalBox(const Hyprutils::Math::Vector2D& monitorPos) const;
        bool                  containsPoint(const Hyprutils::Math::Vector2D& localPos) const;
    };

    inline void SWidgetGeometry::reset() {
        box     = {};
        clipBox = {};
        visible = false;
        dirty   = true;
    }

    inline Hyprutils::Math::CBox SWidgetGeometry::globalBox(const Hyprutils::Math::Vector2D& monitorPos) const {
        return Hyprutils::Math::CBox{box.x + monitorPos.x, box.y + monitorPos.y, box.w, box.h};
    }

    inline bool SWidgetGeometry::containsPoint(const Hyprutils::Math::Vector2D& localPos) const {
        return box.containsPoint(localPos);
    }

}
