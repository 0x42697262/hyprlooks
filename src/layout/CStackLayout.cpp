#include "CStackLayout.hpp"

namespace Hyprlooks {

    void CStackLayout::layout(const Hyprutils::Math::CBox& container, std::vector<IWidget*>& children) {
        for (auto* child : children) {
            if (!child || !child->isVisible())
                continue;
            child->layout(container);
        }
    }

    const char* CStackLayout::name() const {
        return "CStackLayout";
    }

}
