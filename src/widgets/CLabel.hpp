#pragma once

#include "../core/IWidget.hpp"

#include <src/render/Texture.hpp>

#include <string>

namespace Hyprlooks {

class CLabel : public IWidget {
  public:
    CLabel();
    ~CLabel() override = default;

    void                   setText(const std::string& text);
    const std::string&     text() const;

    virtual eWidgetType    type() const override;
    virtual const char*    typeName() const override;
    virtual void           layout(const Hyprutils::Math::CBox& available) override;
    virtual RenderCommandList buildRenderCommands(float monitorScale) override;

  private:
    void                   invalidateCache();
    void                   ensureCache(float monitorScale);

    std::string            m_text;
    SP<Render::ITexture>   m_textTex;
    Hyprutils::Math::Vector2D         m_textSize   = {0.F, 0.F};
    bool                   m_cacheValid = false;
    float                  m_cachedScale = 0.F;
};

}
