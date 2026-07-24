#include "CHyprlandRenderer.hpp"

#include <src/helpers/Monitor.hpp>
#include <src/Compositor.hpp>

void CHyprlandRenderer::damageBox(const Hyprutils::Math::CBox& box) {
    if (!g_pHyprRenderer)
        return;
    g_pHyprRenderer->damageBox(box);
}

void CHyprlandRenderer::scheduleFrame(PHLMONITOR pMonitor) {
    if (!pMonitor)
        return;
    pMonitor->addDamage(CBox{0, 0, pMonitor->m_size.x, pMonitor->m_size.y});
}

void CHyprlandRenderer::scheduleAllFrames() {
    if (!g_pHyprRenderer || !g_pCompositor)
        return;
    for (const auto& monitor : g_pCompositor->m_monitors)
        g_pHyprRenderer->damageMonitor(monitor);
}

void CHyprlandRenderer::addPassElement(UP<IPassElement>&& element) {
    if (!g_pHyprRenderer)
        return;
    g_pHyprRenderer->m_renderPass.add(std::move(element));
}

void CHyprlandRenderer::removePassElements(const std::string& type) {
    if (!g_pHyprRenderer)
        return;
    g_pHyprRenderer->m_renderPass.removeAllOfType(type);
}

SP<Render::ITexture> CHyprlandRenderer::renderText(const std::string& text, const CHyprColor& col, int pt, bool italic, const std::string& fontFamily, int maxWidth) {
    if (!g_pHyprRenderer)
        return nullptr;
    return g_pHyprRenderer->renderText(text, col, pt, italic, fontFamily, maxWidth);
}

SP<Render::ITexture> CHyprlandRenderer::loadAsset(const std::string& filename) {
    if (!g_pHyprRenderer)
        return nullptr;
    return g_pHyprRenderer->loadAsset(filename);
}

SP<Render::ITexture> CHyprlandRenderer::createTextureFromCairo(cairo_surface_t* surface) {
    if (!g_pHyprRenderer || !surface)
        return nullptr;
    return g_pHyprRenderer->createTexture(surface);
}

SP<Render::IFramebuffer> CHyprlandRenderer::createFB(const std::string& name) {
    if (!g_pHyprRenderer)
        return nullptr;
    return g_pHyprRenderer->createFB(name);
}

UP<Render::CScopeGuard> CHyprlandRenderer::bindTempFB(SP<Render::IFramebuffer> fb) {
    if (!g_pHyprRenderer || !fb)
        return nullptr;
    return g_pHyprRenderer->bindTempFB(fb);
}

Hyprutils::Math::CBox CHyprlandRenderer::monitorBox(PHLMONITOR pMonitor) const {
    if (!pMonitor)
        return {};
    return Hyprutils::Math::CBox{pMonitor->m_position.x, pMonitor->m_position.y, pMonitor->m_size.x, pMonitor->m_size.y};
}

float CHyprlandRenderer::monitorScale(PHLMONITOR pMonitor) const {
    if (!pMonitor)
        return 1.F;
    return pMonitor->m_scale;
}
