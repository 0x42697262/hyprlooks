#include "CHyprlandInput.hpp"

Hyprutils::Math::Vector2D CHyprlandInput::cursorPosition() const {
    if (!g_pPointerManager)
        return {};
    return g_pPointerManager->position();
}
