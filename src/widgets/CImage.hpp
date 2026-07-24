#pragma once

#include "../core/IWidget.hpp"

#include <src/render/Texture.hpp>

#include <string>

namespace Hyprlooks {

    class CImage : public IWidget {
      public:
        CImage();
        ~CImage() override = default;

        void                              loadFromFile(const std::string& path);
        void                              setTexture(SP<Render::ITexture> tex);
        void                              setSize(const Hyprutils::Math::Vector2D& size);

        virtual eWidgetType               type() const override;
        virtual const char*               typeName() const override;
        virtual Hyprutils::Math::Vector2D measure() const override;
        virtual void                      layout(const Hyprutils::Math::CBox& available) override;
        virtual RenderCommandList         buildRenderCommands(float monitorScale) override;

      private:
        SP<Render::ITexture>      m_texture;
        std::string               m_path;
        Hyprutils::Math::Vector2D m_size   = {0.F, 0.F};
        bool                      m_loaded = false;
    };

}
