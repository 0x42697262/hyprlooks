#pragma once

#include "ILayoutStrategy.hpp"

namespace Hyprlooks {

    // Absolute layout: every child receives the full container box and is
    // expected to position itself within it (bars, panels, popups). Children
    // overlap in insertion order — later children draw on top. Used by the
    // per-monitor root stage so multiple top-level surfaces can coexist.
    class CStackLayout : public ILayoutStrategy {
      public:
        virtual void        layout(const Hyprutils::Math::CBox& container, std::vector<IWidget*>& children) override;
        virtual const char* name() const override;
    };

}
