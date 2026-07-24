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

    Hyprutils::Math::Vector2D CLabel::measure() const {
        if (m_textSize.x > 0.0 && m_textSize.y > 0.0)
            return m_textSize;

        if (m_text.empty())
            return {0.0, 0.0};

        return {std::max(1.0, sc<double>(m_text.size()) * m_style.fontSize * 0.55), m_style.fontSize * 1.3};
    }

    void CLabel::setStyle(const SWidgetStyle& s) {
        const bool textureChanged = m_style.fgColor != s.fgColor || m_style.fontFamily != s.fontFamily || m_style.fontSize != s.fontSize || m_style.fontItalic != s.fontItalic ||
            m_style.fontWeight != s.fontWeight || m_style.opacity != s.opacity;
        IWidget::setStyle(s);
        if (textureChanged)
            invalidateCache();
    }

    void CLabel::layout(const Hyprutils::Math::CBox& available) {
        m_geometry.box     = available;
        m_geometry.visible = m_visible;
        m_geometry.dirty   = false;
        m_dirty            = false;
        m_styleDirty       = false;
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
        m_cacheValid = false;
        m_textSize   = {0.F, 0.F};
    }

    void CLabel::ensureCache(float monitorScale) {
        if (m_cacheValid && m_cachedScale == monitorScale)
            return;

        if (m_text.empty()) {
            m_textSize    = {0.F, 0.F};
            m_cacheValid  = true;
            m_cachedScale = monitorScale;
            return;
        }

        static CHyprlandRenderer renderer;

        const int                fontSizePx = std::max(8, sc<int>(m_style.fontSize * monitorScale));
        const auto&              fontFamily = m_style.fontFamily.empty() ? std::string{} : m_style.fontFamily;

        const auto               oldSize = m_textSize;
        auto                     tex     = renderer.renderText(m_text, m_style.fgColor, fontSizePx, m_style.fontItalic, fontFamily);
        if (tex && tex->ok()) {
            m_textTex  = tex;
            m_textSize = tex->m_size / monitorScale;
        } else {
            m_textTex.reset();
            m_textSize = {0.F, 0.F};
        }

        m_cacheValid  = true;
        m_cachedScale = monitorScale;

        if (oldSize != m_textSize)
            markDirty();
    }

}
