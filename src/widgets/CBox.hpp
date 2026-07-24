#pragma once

#include "../core/IWidget.hpp"

namespace Hyprlooks {

    class CBox : public IWidget {
      public:
        CBox();
        ~CBox() override = default;

        virtual eWidgetType               type() const override;
        virtual const char*               typeName() const override;
        virtual Hyprutils::Math::Vector2D measure() const override;
        virtual void                      layout(const Hyprutils::Math::CBox& available) override;
        virtual RenderCommandList         buildRenderCommands(float monitorScale) override;
    };

}
