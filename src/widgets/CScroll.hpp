#pragma once

#include "CContainer.hpp"

namespace Hyprlooks {

    // A vertically scrollable container. Children are stacked top-to-bottom and
    // shifted by a scroll offset driven by the mouse wheel; overflowing content
    // is clipped to the scroll viewport (the widget's own box).
    class CScroll : public CContainer {
      public:
        CScroll();
        ~CScroll() override = default;

        void                              setGap(float gap);
        void                              setFixedHeight(float h);
        void                              scrollBy(double dy);

        virtual eWidgetType               type() const override;
        virtual const char*               typeName() const override;
        virtual Hyprutils::Math::Vector2D measure() const override;
        virtual void                      layout(const Hyprutils::Math::CBox& available) override;
        virtual RenderCommandList         buildRenderCommands(float monitorScale) override;

      private:
        void   clampScroll();

        float  m_gap            = 6.F;
        float  m_fixedHeight    = 0.F; // viewport height; 0 == size to content (no scroll)
        double m_scrollY        = 0.0;
        double m_contentHeight  = 0.0;
        double m_viewportHeight = 0.0;
    };

}
