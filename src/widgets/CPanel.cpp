#include "CPanel.hpp"
#include "../theme/CThemeDefaults.hpp"

namespace Hyprlooks {

    // sensible fallback thicknesses when a panel omits an explicit size
    static constexpr float DEFAULT_EDGE_THICKNESS = 64.F;
    static constexpr float DEFAULT_CENTER_W        = 480.F;
    static constexpr float DEFAULT_CENTER_H        = 320.F;

    CPanel::CPanel() {
        m_style = CThemeDefaults::panelStyle();
    }

    eWidgetType CPanel::type() const {
        return eWidgetType::PANEL;
    }

    const char* CPanel::typeName() const {
        return "CPanel";
    }

    void CPanel::setAnchor(ePanelAnchor anchor) {
        m_anchor = anchor;
        markDirty();
    }

    void CPanel::setPanelSize(float w, float h) {
        m_panelWidth  = w;
        m_panelHeight = h;
        markDirty();
    }

    void CPanel::setMargin(const Hyprutils::Math::Vector2D& margin) {
        m_margin = margin;
        markDirty();
    }

    ePanelAnchor CPanel::anchor() const {
        return m_anchor;
    }

    float CPanel::panelWidth() const {
        return m_panelWidth;
    }

    float CPanel::panelHeight() const {
        return m_panelHeight;
    }

    const Hyprutils::Math::Vector2D& CPanel::margin() const {
        return m_margin;
    }

    Hyprutils::Math::CBox CPanel::computeBox(const Hyprutils::Math::CBox& monitor) const {
        const float mx     = m_margin.x;
        const float my     = m_margin.y;
        const float availW = monitor.w - mx * 2.F;
        const float availH = monitor.h - my * 2.F;

        float       w = m_panelWidth;
        float       h = m_panelHeight;
        float       x = monitor.x + mx;
        float       y = monitor.y + my;

        switch (m_anchor) {
            case ePanelAnchor::LEFT:
                if (w <= 0.F)
                    w = DEFAULT_EDGE_THICKNESS;
                if (h <= 0.F)
                    h = availH;
                x = monitor.x + mx;
                y = monitor.y + my;
                break;
            case ePanelAnchor::RIGHT:
                if (w <= 0.F)
                    w = DEFAULT_EDGE_THICKNESS;
                if (h <= 0.F)
                    h = availH;
                x = monitor.x + monitor.w - w - mx;
                y = monitor.y + my;
                break;
            case ePanelAnchor::TOP:
                if (h <= 0.F)
                    h = DEFAULT_EDGE_THICKNESS;
                if (w <= 0.F)
                    w = availW;
                x = monitor.x + mx;
                y = monitor.y + my;
                break;
            case ePanelAnchor::BOTTOM:
                if (h <= 0.F)
                    h = DEFAULT_EDGE_THICKNESS;
                if (w <= 0.F)
                    w = availW;
                x = monitor.x + mx;
                y = monitor.y + monitor.h - h - my;
                break;
            case ePanelAnchor::CENTER:
                if (w <= 0.F)
                    w = DEFAULT_CENTER_W;
                if (h <= 0.F)
                    h = DEFAULT_CENTER_H;
                x = monitor.x + (monitor.w - w) / 2.F;
                y = monitor.y + (monitor.h - h) / 2.F;
                break;
            case ePanelAnchor::FILL:
            default:
                w = availW;
                h = availH;
                x = monitor.x + mx;
                y = monitor.y + my;
                break;
        }

        return {x, y, w, h};
    }

    void CPanel::layout(const Hyprutils::Math::CBox& available) {
        // available is the full monitor box handed down by the root stage.
        const auto box = computeBox(available);
        CContainer::layout(box);
    }

}
