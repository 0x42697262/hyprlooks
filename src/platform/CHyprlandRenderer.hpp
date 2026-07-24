#pragma once

#include <src/defines.hpp>
#include <src/render/Renderer.hpp>
#include <src/render/Texture.hpp>
#include <src/render/Framebuffer.hpp>

#include <hyprutils/math/Box.hpp>

#include <string>
#include <functional>

class CHyprlandRenderer {
  public:
    CHyprlandRenderer()  = default;
    ~CHyprlandRenderer() = default;

    void                     damageBox(const Hyprutils::Math::CBox& box);
    void                     scheduleFrame(PHLMONITOR pMonitor);
    void                     scheduleAllFrames();
    void                     addPassElement(UP<IPassElement>&& element);
    void                     removePassElements(const std::string& type);
    SP<Render::ITexture>     renderText(const std::string& text, const CHyprColor& col, int pt, bool italic = false, const std::string& fontFamily = "", int maxWidth = 0);
    SP<Render::ITexture>     loadAsset(const std::string& filename);
    SP<Render::ITexture>     createTextureFromCairo(cairo_surface_t* surface);
    SP<Render::IFramebuffer> createFB(const std::string& name = "");
    UP<Render::CScopeGuard>  bindTempFB(SP<Render::IFramebuffer> fb);

    Hyprutils::Math::CBox    monitorBox(PHLMONITOR pMonitor) const;
    float                    monitorScale(PHLMONITOR pMonitor) const;
};
