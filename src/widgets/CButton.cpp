#include "CButton.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../state/CNormalState.hpp"
#include "../state/CHoverState.hpp"
#include "../state/CPressedState.hpp"
#include "../render/commands/CDrawRectCommand.hpp"

namespace Hyprlooks {

CButton::CButton() {
    m_style = CThemeDefaults::buttonStyle();
    setState(makeUnique<CNormalState>());
}

void CButton::setText(const std::string& text) {
    m_label.setText(text);
    m_label.setStyle(m_style);
    markDirty();
}

void CButton::setIcon(const std::string& icon) {
    m_label.setText(icon);
    m_label.setStyle(m_style);
    markDirty();
}

void CButton::onClick(ClickCallback cb) {
    m_clickCallback = std::move(cb);
}

eWidgetType CButton::type() const {
    return eWidgetType::BUTTON;
}

const char* CButton::typeName() const {
    return "CButton";
}

bool CButton::interactive() const {
    return true;
}

void CButton::layout(const Hyprutils::Math::CBox& available) {
    m_geometry.box     = available;
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;

    auto inner = available;
    inner.x += m_style.padding.x;
    inner.y += m_style.padding.y;
    inner.w -= m_style.padding.x * 2.F;
    inner.h -= m_style.padding.y * 2.F;

    m_label.setStyle(m_style);
    m_label.layout(inner);
}

IWidget::RenderCommandList CButton::buildRenderCommands(float monitorScale) {
    RenderCommandList commands;

    SWidgetStyle effectiveStyle = m_style;
    if (m_state)
        effectiveStyle = m_state->styleFor(m_style);

    if (effectiveStyle.bgColor.a > 0.F) {
        commands.push_back(makeUnique<CDrawRectCommand>(CDrawRectCommand::SParams{
            .box          = m_geometry.box,
            .color        = effectiveStyle.bgColor,
            .round        = sc<int>(effectiveStyle.borderRadius.topLeft),
            .roundingPower = effectiveStyle.roundingPower,
            .blur         = effectiveStyle.blur,
            .blurAlpha    = effectiveStyle.blurAlpha,
        }));
    }

    auto labelCommands = m_label.buildRenderCommands(monitorScale);
    for (auto& cmd : labelCommands)
        commands.push_back(std::move(cmd));

    return commands;
}

void CButton::fireClick() {
    if (m_clickCallback)
        m_clickCallback();
}

void CButton::applyStateStyle() {
    markDirty();
}

}
