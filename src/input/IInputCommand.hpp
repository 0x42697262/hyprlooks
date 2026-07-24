#pragma once

#include "../core/IWidget.hpp"

#include <src/defines.hpp>
#include <src/devices/IPointer.hpp>
#include <src/devices/IKeyboard.hpp>

#include <hyprutils/math/Vector2D.hpp>

#include <string>

namespace Hyprlooks {

    class IInputCommand {
      public:
        virtual ~IInputCommand() = default;

        virtual void        execute()    = 0;
        virtual const char* name() const = 0;
    };

}
