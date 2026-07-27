#pragma once

#include <hyprutils/memory/UniquePtr.hpp>

#include <cstdint>
#include <string>

struct xkb_context;
struct xkb_keymap;
struct xkb_state;

namespace Hyprlooks {

    // Self-contained xkb translation of raw evdev keycodes to text / keysyms.
    //
    // A plugin cannot reach the compositor keyboard's (protected) xkb_state, so
    // we keep our own state seeded from the default system keymap and feed it
    // every key event to track modifiers (shift, caps, ...). The layout follows
    // the system default (XKB_DEFAULT_* env, else "us"); it does not track
    // per-window Hyprland layout switching. Good enough for text fields.
    class CKeyboardTranslator {
      public:
        CKeyboardTranslator();
        ~CKeyboardTranslator();

        CKeyboardTranslator(const CKeyboardTranslator&)            = delete;
        CKeyboardTranslator& operator=(const CKeyboardTranslator&) = delete;

        // Feed every key event so modifier state stays correct.
        void        updateKey(uint32_t keycode, bool pressed);
        // UTF-8 text produced by this key given current modifiers ("" if none).
        std::string utf8(uint32_t keycode) const;
        // xkb_keysym_t for the key (for Backspace/Enter/Escape/... handling).
        uint32_t    keysym(uint32_t keycode) const;

      private:
        xkb_context* m_ctx    = nullptr;
        xkb_keymap*  m_keymap = nullptr;
        xkb_state*   m_state  = nullptr;
    };

    UP<CKeyboardTranslator>& keyboardTranslator();

}
