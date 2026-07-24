#include "CBox.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../render/commands/CDrawRectCommand.hpp"
#include "../render/commands/CDrawBorderCommand.hpp"

namespace Hyprlooks {

CBox::CBox() {
    m_style = CThemeDefaults::boxStyle();
}

eWidgetType CBox::type() const {
    return eWidgetType::BOX;
}

const char* CBox::typeName() const {
    return "CBox";
}

void CBox::layout(const Hyprutils::Math::CBox& available) {
    m_geometry.box     = available;
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;
}

IWidget::RenderCommandList CBox::buildRenderCommands(float monitorScale) {
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

    if (m_style.border.width > 0.F && m_style.border.color.a > 0.F) {
        commands.push_back(makeUnique<CDrawBorderCommand>(CDrawBorderCommand::SParams{
            .box          = m_geometry.box,
            .width        = m_style.border.width,
            .color        = m_style.border.color,
            .round        = sc<int>(m_style.borderRadius.topLeft),
            .roundingPower = m_style.roundingPower,
        }));
    }

    return commands;
}

}
