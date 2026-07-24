#include "CImage.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../platform/CHyprlandRenderer.hpp"
#include "../render/commands/CDrawTextureCommand.hpp"

namespace Hyprlooks {

CImage::CImage() {
    m_style = CThemeDefaults::imageStyle();
}

void CImage::loadFromFile(const std::string& path) {
    m_path  = path;
    m_loaded = false;
    markDirty();
}

void CImage::setTexture(SP<Render::ITexture> tex) {
    m_texture = tex;
    m_loaded  = true;
    markDirty();
}

eWidgetType CImage::type() const {
    return eWidgetType::IMAGE;
}

const char* CImage::typeName() const {
    return "CImage";
}

void CImage::layout(const Hyprutils::Math::CBox& available) {
    m_geometry.box     = available;
    m_geometry.visible = m_visible;
    m_geometry.dirty   = false;
}

IWidget::RenderCommandList CImage::buildRenderCommands(float monitorScale) {
    if (!m_loaded && !m_path.empty()) {
        static CHyprlandRenderer renderer;
        m_texture = renderer.loadAsset(m_path);
        m_loaded  = true;
    }

    RenderCommandList commands;

    if (m_texture && m_texture->ok()) {
        commands.push_back(makeUnique<CDrawTextureCommand>(CDrawTextureCommand::SParams{
            .tex   = m_texture,
            .box   = m_geometry.box,
            .alpha = m_style.opacity,
        }));
    }

    return commands;
}

}
