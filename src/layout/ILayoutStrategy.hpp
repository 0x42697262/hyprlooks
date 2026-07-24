#pragma once

#include "../core/IWidget.hpp"

#include <hyprutils/math/Box.hpp>

#include <vector>

namespace Hyprlooks {

enum class eFlexDirection : uint8_t {
    ROW    = 0,
    COLUMN = 1,
};

enum class eFlexJustify : uint8_t {
    START    = 0,
    CENTER   = 1,
    END      = 2,
    SPACE_BETWEEN = 3,
};

enum class eFlexAlign : uint8_t {
    START  = 0,
    CENTER = 1,
    END    = 2,
    STRETCH = 3,
};

class ILayoutStrategy {
  public:
    virtual ~ILayoutStrategy() = default;

    virtual void          layout(const Hyprutils::Math::CBox& container, std::vector<IWidget*>& children) = 0;
    virtual const char*   name() const = 0;
};

}
