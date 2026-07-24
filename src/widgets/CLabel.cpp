#include "CLabel.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../platform/CHyprlandRenderer.hpp"
#include "../render/commands/CDrawTextureCommand.hpp"

namespace Hyprlooks {

CLabel::CLabel() {
    m_style = CThemeDefaults::labelStyle();
}

void CLabel::setText(const std::string& text) {
    if (m_text == text)
        return;
    m_text = text;
    invalidateCache();
    markDirty();
}

const std::string& CLabel::text() const {
    return m_text;
}

eWidgetType CLabel::type() const {
    return eWidgetType::LABEL;
}

const char* CLabel::typeName() const {
    return "CLabel";
}

void CLabel::layout(const Hyprutils::Math::CBox& available) {
    m_geometry.box     = available;
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;
}

IWidget::RenderCommandList CLabel::buildRenderCommands(float monitorScale) {
    ensureCache(monitorScale);

    RenderCommandList commands;

    if (m_textTex && m_textTex->ok() && m_textSize.x > 0 && m_textSize.y > 0) {
        auto box = m_geometry.box;
        box.w    = m_textSize.x;
        box.h    = m_textSize.y;

        commands.push_back(makeUnique<CDrawTextureCommand>(CDrawTextureCommand::SParams{
            .tex   = m_textTex,
            .box   = box,
            .alpha = m_style.opacity,
        }));
    }

    return commands;
}

void CLabel::invalidateCache() {
    m_textTex.reset();
    m_cacheValid  = false;
    m_textSize    = {0.F, 0.F};
}

void CLabel::ensureCache(float monitorScale) {
    if (m_cacheValid && m_cachedScale == monitorScale)
        return;

    if (m_text.empty()) {
        m_textSize = {0.F, 0.F};
        m_cacheValid = true;
        m_cachedScale = monitorScale;
        return;
    }

    static CHyprlandRenderer renderer;

    const int   fontSizePx = std::clamp(sc<int>(m_style.fontSize * monitorScale), 8, 40);
    const auto& fontFamily  = m_style.fontFamily.empty() ? std::string{} : m_style.fontFamily;

    auto tex = renderer.renderText(m_text, m_style.fgColor, fontSizePx, m_style.fontItalic, fontFamily);
    if (tex && tex->ok()) {
        m_textTex   = tex;
        m_textSize  = tex->m_size / monitorScale;
    } else {
        m_textTex.reset();
        m_textSize  = {0.F, 0.F};
    }

    m_cacheValid  = true;
    m_cachedScale = monitorScale;
}

}
