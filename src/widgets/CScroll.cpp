#include "CScroll.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../render/commands/CDrawRectCommand.hpp"

#include <algorithm>
#include <vector>

namespace Hyprlooks {

    CScroll::CScroll() {
        m_style          = CThemeDefaults::containerStyle();
        m_style.bgColor.a = 0.F; // transparent viewport by default
        m_style.padding  = {4.F, 4.F};
    }

    void CScroll::setGap(float gap) {
        m_gap = gap;
        markDirty();
    }

    void CScroll::setFixedHeight(float h) {
        m_fixedHeight = h;
        markDirty();
    }

    Hyprutils::Math::Vector2D CScroll::measure() const {
        double maxW    = 0.0;
        double content = 0.0;
        size_t n       = 0;
        for (const auto& child : m_children) {
            if (!child->isVisible())
                continue;
            const auto sz = child->measure();
            maxW          = std::max(maxW, sz.x);
            content += sz.y + m_gap;
            ++n;
        }
        if (n > 0)
            content -= m_gap;

        const double h = m_fixedHeight > 0.F ? sc<double>(m_fixedHeight) : content;
        return {maxW + m_style.padding.x * 2.0, h + m_style.padding.y * 2.0};
    }

    eWidgetType CScroll::type() const {
        return eWidgetType::SCROLL;
    }

    const char* CScroll::typeName() const {
        return "CScroll";
    }

    void CScroll::scrollBy(double dy) {
        const double before = m_scrollY;
        m_scrollY += dy;
        clampScroll();
        if (m_scrollY != before)
            markDirty();
    }

    void CScroll::clampScroll() {
        const double maxScroll = std::max(0.0, m_contentHeight - m_viewportHeight);
        m_scrollY              = std::clamp(m_scrollY, 0.0, maxScroll);
    }

    void CScroll::layout(const Hyprutils::Math::CBox& available) {
        m_geometry.box     = available;
        m_geometry.visible = m_visible;
        m_geometry.dirty   = false;
        m_dirty            = false;
        m_styleDirty       = false;

        const double padX   = m_style.padding.x;
        const double padY   = m_style.padding.y;
        const double innerX = available.x + padX;
        const double innerW = available.w - padX * 2.0;
        m_viewportHeight    = available.h - padY * 2.0;

        // phase 1: measure visible children to know the content height
        std::vector<IWidget*> visible;
        std::vector<double>   heights;
        visible.reserve(m_children.size());
        heights.reserve(m_children.size());

        double content = 0.0;
        for (auto& child : m_children) {
            if (!child->isVisible())
                continue;
            const double h = child->measure().y;
            visible.push_back(child.get());
            heights.push_back(h);
            content += h + m_gap;
        }
        if (!heights.empty())
            content -= m_gap; // no trailing gap

        m_contentHeight = content;
        clampScroll();

        // phase 2: place children, offset by the (clamped) scroll position
        double cursorY = 0.0;
        for (size_t i = 0; i < visible.size(); ++i) {
            Hyprutils::Math::CBox cbox = {innerX, available.y + padY + cursorY - m_scrollY, innerW, heights[i]};
            visible[i]->layout(cbox);
            cursorY += heights[i] + m_gap;
        }
    }

    IWidget::RenderCommandList CScroll::buildRenderCommands(float monitorScale) {
        RenderCommandList commands;

        if (m_style.bgColor.a > 0.F) {
            commands.push_back(makeUnique<CDrawRectCommand>(CDrawRectCommand::SParams{
                .box           = m_geometry.box,
                .color         = m_style.bgColor,
                .round         = sc<int>(m_style.borderRadius.topLeft),
                .roundingPower = m_style.roundingPower,
                .blur          = m_style.blur,
                .blurAlpha     = m_style.blurAlpha,
            }));
        }

        // clip every child command to the viewport so scrolled content does not
        // bleed outside the scroll area.
        const auto viewport = m_geometry.box;
        for (auto& child : m_children) {
            if (!child->isVisible())
                continue;
            auto childCommands = child->buildRenderCommands(monitorScale);
            for (auto& cmd : childCommands) {
                cmd->setClip(viewport);
                commands.push_back(std::move(cmd));
            }
        }

        return commands;
    }

}
