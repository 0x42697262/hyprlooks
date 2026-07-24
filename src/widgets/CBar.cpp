#include "CBar.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../layout/CFlexLayout.hpp"
#include "../render/commands/CDrawRectCommand.hpp"
#include "../state/IWidgetState.hpp"

namespace Hyprlooks {

CBar::CBar() {
    m_style = CThemeDefaults::barStyle();
    ensureSections();
}

void CBar::setMonitorName(const std::string& name) {
    m_monitorName = name;
}

const std::string& CBar::monitorName() const {
    return m_monitorName;
}

void CBar::setPosition(eBarPosition pos) {
    m_position = pos;
    markDirty();
}

eBarPosition CBar::position() const {
    return m_position;
}

void CBar::setHeight(float h) {
    m_height = h;
    markDirty();
}

float CBar::height() const {
    return m_height;
}

void CBar::setWidth(float w) {
    m_width = w;
    markDirty();
}

float CBar::width() const {
    return m_width;
}

CContainer* CBar::leftSection() {
    return m_left.get();
}

CContainer* CBar::centerSection() {
    return m_center.get();
}

CContainer* CBar::rightSection() {
    return m_right.get();
}

eWidgetType CBar::type() const {
    return eWidgetType::BAR;
}

const char* CBar::typeName() const {
    return "CBar";
}

void CBar::layout(const Hyprutils::Math::CBox& available) {
    ensureSections();

    float barW = m_width > 0 ? m_width : available.w;
    float barH = m_height;
    float barX = available.x;
    float barY = available.y;

    if (m_position == eBarPosition::BOTTOM)
        barY = available.y + available.h - barH;
    if (m_position == eBarPosition::LEFT)
        barW = m_width > 0 ? m_width : available.w * 0.1F;
    if (m_position == eBarPosition::RIGHT) {
        barX = available.x + available.w - (m_width > 0 ? m_width : available.w * 0.1F);
        barW = m_width > 0 ? m_width : available.w * 0.1F;
    }

    m_geometry.box     = {barX, barY, barW, barH};
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;

    SFlexOptions flexOpts;
    flexOpts.direction = eFlexDirection::ROW;
    flexOpts.justify   = eFlexJustify::START;
    flexOpts.align     = eFlexAlign::CENTER;
    flexOpts.padding   = m_style.padding;

    auto inner = m_geometry.box;
    inner.x += m_style.padding.x;
    inner.y += m_style.padding.y;
    inner.w -= m_style.padding.x * 2.F;
    inner.h -= m_style.padding.y * 2.F;

    std::vector<IWidget*> sections;
    if (m_left && m_left->childCount() > 0)
        sections.push_back(m_left.get());
    if (m_center && m_center->childCount() > 0)
        sections.push_back(m_center.get());
    if (m_right && m_right->childCount() > 0)
        sections.push_back(m_right.get());

    if (!sections.empty()) {
        CFlexLayout flex(flexOpts);
        flex.layout(inner, sections);
    }
}

IWidget::RenderCommandList CBar::buildRenderCommands(float monitorScale) {
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

    auto buildSection = [&](CContainer* section) {
        if (!section || section->childCount() == 0)
            return;
        auto sectionCommands = section->buildRenderCommands(monitorScale);
        for (auto& cmd : sectionCommands)
            commands.push_back(std::move(cmd));
    };

    buildSection(m_left.get());
    buildSection(m_center.get());
    buildSection(m_right.get());

    return commands;
}

void CBar::ensureSections() {
    if (m_sectionsBuilt)
        return;

    SFlexOptions leftOpts;
    leftOpts.direction = eFlexDirection::ROW;
    leftOpts.justify   = eFlexJustify::START;
    leftOpts.align     = eFlexAlign::CENTER;
    leftOpts.mainGap   = 4.F;

    SFlexOptions centerOpts;
    centerOpts.direction = eFlexDirection::ROW;
    centerOpts.justify   = eFlexJustify::CENTER;
    centerOpts.align     = eFlexAlign::CENTER;
    centerOpts.mainGap   = 4.F;

    SFlexOptions rightOpts;
    rightOpts.direction = eFlexDirection::ROW;
    rightOpts.justify   = eFlexJustify::END;
    rightOpts.align     = eFlexAlign::CENTER;
    rightOpts.mainGap   = 4.F;

    m_left   = makeUnique<CContainer>();
    m_center = makeUnique<CContainer>();
    m_right  = makeUnique<CContainer>();

    m_left->setLayoutStrategy(makeUnique<CFlexLayout>(leftOpts));
    m_center->setLayoutStrategy(makeUnique<CFlexLayout>(centerOpts));
    m_right->setLayoutStrategy(makeUnique<CFlexLayout>(rightOpts));

    m_sectionsBuilt = true;
}

}
