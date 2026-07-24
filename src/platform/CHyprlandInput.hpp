#pragma once

#include <src/defines.hpp>
#include <src/managers/PointerManager.hpp>

class CHyprlandInput {
  public:
    CHyprlandInput()  = default;
    ~CHyprlandInput() = default;

    Hyprutils::Math::Vector2D cursorPosition() const;
};
