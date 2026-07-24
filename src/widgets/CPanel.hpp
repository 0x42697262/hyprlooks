#pragma once

#include "CContainer.hpp"

namespace Hyprlooks {

    enum class ePanelAnchor : uint8_t {
        FILL   = 0,
        TOP    = 1,
        BOTTOM = 2,
        LEFT   = 3,
        RIGHT  = 4,
        CENTER = 5,
    };

    // A free-floating, self-positioning container. Unlike a plain container
    // (which fills whatever box it is handed), a panel computes its own box
    // from an anchor + explicit size + margin against the monitor area, then
    // lays its children out inside that box. This is the building block for
    // Vivaldi-style side docks, status strips and launcher popups.
    //
    // type() intentionally stays CONTAINER so hit-testing and rendering treat
    // it like any other container subtree.
    class CPanel : public CContainer {
      public:
        CPanel();
        ~CPanel() override = default;

        void                             setAnchor(ePanelAnchor anchor);
        void                             setPanelSize(float w, float h);
        void                             setMargin(const Hyprutils::Math::Vector2D& margin);

        ePanelAnchor                     anchor() const;
        float                            panelWidth() const;
        float                            panelHeight() const;
        const Hyprutils::Math::Vector2D& margin() const;

        virtual eWidgetType type() const override;
        virtual const char* typeName() const override;
        virtual void        layout(const Hyprutils::Math::CBox& available) override;

      private:
        Hyprutils::Math::CBox     computeBox(const Hyprutils::Math::CBox& monitor) const;

        ePanelAnchor              m_anchor      = ePanelAnchor::LEFT;
        float                     m_panelWidth  = 0.F;
        float                     m_panelHeight = 0.F;
        Hyprutils::Math::Vector2D m_margin      = {0.F, 0.F};
    };

}
