#include "CKeyboardTranslator.hpp"

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-keysyms.h>

namespace Hyprlooks {

    // evdev keycodes are offset by 8 from xkb keycodes.
    static constexpr uint32_t EVDEV_XKB_OFFSET = 8;

    CKeyboardTranslator::CKeyboardTranslator() {
        m_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        if (!m_ctx)
            return;

        // all-null rule names => compile the system default keymap
        const xkb_rule_names names = {nullptr, nullptr, nullptr, nullptr, nullptr};
        m_keymap                   = xkb_keymap_new_from_names(m_ctx, &names, XKB_KEYMAP_COMPILE_NO_FLAGS);
        if (!m_keymap)
            return;

        m_state = xkb_state_new(m_keymap);
    }

    CKeyboardTranslator::~CKeyboardTranslator() {
        if (m_state)
            xkb_state_unref(m_state);
        if (m_keymap)
            xkb_keymap_unref(m_keymap);
        if (m_ctx)
            xkb_context_unref(m_ctx);
    }

    void CKeyboardTranslator::updateKey(uint32_t keycode, bool pressed) {
        if (!m_state)
            return;
        xkb_state_update_key(m_state, keycode + EVDEV_XKB_OFFSET, pressed ? XKB_KEY_DOWN : XKB_KEY_UP);
    }

    std::string CKeyboardTranslator::utf8(uint32_t keycode) const {
        if (!m_state)
            return {};

        char      buf[64];
        const int n = xkb_state_key_get_utf8(m_state, keycode + EVDEV_XKB_OFFSET, buf, sizeof(buf));
        if (n <= 0)
            return {};
        return std::string(buf, static_cast<size_t>(n));
    }

    uint32_t CKeyboardTranslator::keysym(uint32_t keycode) const {
        if (!m_state)
            return XKB_KEY_NoSymbol;
        return xkb_state_key_get_one_sym(m_state, keycode + EVDEV_XKB_OFFSET);
    }

    UP<CKeyboardTranslator>& keyboardTranslator() {
        static UP<CKeyboardTranslator> t = makeUnique<CKeyboardTranslator>();
        return t;
    }

}
