#include "CFlexLayout.hpp"

#include <algorithm>

namespace Hyprlooks {

CFlexLayout::CFlexLayout(SFlexOptions opts) : m_opts(std::move(opts)) {
    ;
}

void CFlexLayout::layout(const Hyprutils::Math::CBox& container, std::vector<IWidget*>& children) {
    if (children.empty())
        return;

    const float padX    = m_opts.padding.x;
    const float padY    = m_opts.padding.y;
    const float innerW  = container.w - padX * 2.F;
    const float innerH  = container.h - padY * 2.F;
    const float startX  = container.x + padX;
    const float startY  = container.y + padY;

    const bool  isRow     = m_opts.direction == eFlexDirection::ROW;
    const float mainSize  = isRow ? innerW : innerH;
    const float crossSize = isRow ? innerH : innerW;
    float       gap       = m_opts.mainGap;

    float totalChildSize = 0.F;
    for (auto* child : children) {
        const auto& g = child->geometry().box;
        totalChildSize += isRow ? g.w : g.h;
    }
    totalChildSize += gap * sc<float>(children.size() - 1);

    float offset = 0.F;
    if (m_opts.justify == eFlexJustify::CENTER)
        offset = (mainSize - totalChildSize) / 2.F;
    else if (m_opts.justify == eFlexJustify::END)
        offset = mainSize - totalChildSize;
    else if (m_opts.justify == eFlexJustify::SPACE_BETWEEN && children.size() > 1)
        gap = (mainSize - totalChildSize + gap * sc<float>(children.size() - 1)) / sc<float>(children.size() - 1);

    for (auto* child : children) {
        if (!child->isVisible())
            continue;

        const auto& childGeom = child->geometry().box;
        float       childMain  = isRow ? childGeom.w : childGeom.h;
        float       childCross = isRow ? childGeom.h : childGeom.w;

        float crossPos = 0.F;
        if (m_opts.align == eFlexAlign::CENTER)
            crossPos = (crossSize - childCross) / 2.F;
        else if (m_opts.align == eFlexAlign::END)
            crossPos = crossSize - childCross;
        else if (m_opts.align == eFlexAlign::STRETCH)
            childCross = crossSize;

        Hyprutils::Math::CBox box;
        if (isRow) {
            box = {startX + offset, startY + crossPos, childMain, childCross};
        } else {
            box = {startX + crossPos, startY + offset, childCross, childMain};
        }

        child->layout(box);
        offset += childMain + gap;
    }
}

const char* CFlexLayout::name() const {
    return "CFlexLayout";
}

}
