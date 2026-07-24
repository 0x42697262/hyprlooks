#include "CFocusManager.hpp"

namespace Hyprlooks {

    void CFocusManager::setFocused(WP<IWidget> widget) {
        m_focused = widget;
    }

    WP<IWidget> CFocusManager::focused() const {
        return m_focused;
    }

    void CFocusManager::clearFocus() {
        m_focused.reset();
    }

    bool CFocusManager::hasFocus() const {
        // weak ptr over a UP-owned widget cannot be locked; query validity.
        return m_focused.valid();
    }

}
