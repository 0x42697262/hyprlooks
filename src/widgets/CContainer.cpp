#include "CContainer.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../render/commands/CDrawRectCommand.hpp"

namespace Hyprlooks {

CContainer::CContainer() {
    m_style = CThemeDefaults::containerStyle();
}

void CContainer::addChild(UP<IWidget> child) {
    if (child)
        m_children.push_back(std::move(child));
    markDirty();
}

void CContainer::clearChildren() {
    m_children.clear();
    markDirty();
}

size_t CContainer::childCount() const {
    return m_children.size();
}

void CContainer::setLayoutStrategy(UP<ILayoutStrategy> strategy) {
    m_layoutStrategy = std::move(strategy);
    markDirty();
}

ILayoutStrategy* CContainer::layoutStrategy() const {
    return m_layoutStrategy.get();
}

eWidgetType CContainer::type() const {
    return eWidgetType::CONTAINER;
}

const char* CContainer::typeName() const {
    return "CContainer";
}

void CContainer::layout(const Hyprutils::Math::CBox& available) {
    m_geometry.box     = available;
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;

    if (!m_layoutStrategy || m_children.empty())
        return;

    std::vector<IWidget*> children;
    children.reserve(m_children.size());
    for (auto& child : m_children)
        children.push_back(child.get());

    auto inner = available;
    inner.x += m_style.padding.x;
    inner.y += m_style.padding.y;
    inner.w -= m_style.padding.x * 2.F;
    inner.h -= m_style.padding.y * 2.F;

    m_layoutStrategy->layout(inner, children);
}

IWidget::RenderCommandList CContainer::buildRenderCommands(float monitorScale) {
    RenderCommandList commands;

    if (m_style.bgColor.a > 0.F) {
        commands.push_back(makeUnique<CDrawRectCommand>(CDrawRectCommand::SParams{
            .box          = m_geometry.box,
            .color        = m_style.bgColor,
            .round        = sc<int>(m_style.borderRadius.topLeft),
            .roundingPower = m_style.roundingPower,
            .blur         = m_style.blur,
            .blurAlpha    = m_style.blurAlpha,
        }));
    }

    for (auto& child : m_children) {
        if (!child->isVisible())
            continue;
        auto childCommands = child->buildRenderCommands(monitorScale);
        for (auto& cmd : childCommands)
            commands.push_back(std::move(cmd));
    }

    return commands;
}

}
