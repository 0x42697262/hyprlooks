#include "IWidget.hpp"
#include "../state/IWidgetState.hpp"
#include "../render/IRenderCommand.hpp"

namespace Hyprlooks {

    void IWidget::layout(const Hyprutils::Math::CBox& available) {
        m_geometry.box     = available;
        m_geometry.dirty   = false;
        m_geometry.visible = m_visible;
        m_dirty            = false;
        m_styleDirty       = false;
    }

    bool IWidget::hitTest(const Hyprutils::Math::Vector2D& localPos) const {
        return m_geometry.containsPoint(localPos);
    }

    bool IWidget::interactive() const {
        return false;
    }

    void IWidget::setState(UP<IWidgetState>&& state) {
        if (m_state)
            m_state->onExit(*this);
        m_state = std::move(state);
        if (m_state)
            m_state->onEnter(*this);
        markDirty();
    }

    IWidgetState* IWidget::state() const {
        return m_state.get();
    }

    const SWidgetGeometry& IWidget::geometry() const {
        return m_geometry;
    }

    SWidgetGeometry& IWidget::geometry() {
        return m_geometry;
    }

    const SWidgetStyle& IWidget::style() const {
        return m_style;
    }

    void IWidget::setStyle(const SWidgetStyle& s) {
        m_style      = s;
        m_styleDirty = true;
        markDirty();
    }

    void IWidget::setParent(IWidget* parent) {
        m_parent = parent;
    }

    IWidget* IWidget::parent() const {
        return m_parent;
    }

    void IWidget::setSelf(WP<IWidget> self) {
        m_self = std::move(self);
    }

    WP<IWidget> IWidget::self() const {
        return m_self;
    }

    void IWidget::markDirty() {
        m_dirty = true;
        if (m_parent)
            m_parent->markDirty();
    }

    bool IWidget::isDirty() const {
        return m_dirty;
    }

    void IWidget::setVisible(bool visible) {
        m_visible = visible;
        markDirty();
    }

    bool IWidget::isVisible() const {
        return m_visible;
    }

    void IWidget::setStyleDirty() {
        m_styleDirty = true;
        markDirty();
    }

    bool IWidget::isStyleDirty() const {
        return m_styleDirty;
    }

    void IWidget::setId(const std::string& id) {
        m_id = id;
    }

    const std::string& IWidget::id() const {
        return m_id;
    }

}
