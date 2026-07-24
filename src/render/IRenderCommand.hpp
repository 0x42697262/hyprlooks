#pragma once

#include <src/defines.hpp>
#include <src/render/pass/PassElement.hpp>

#include <hyprutils/math/Box.hpp>
#include <hyprutils/math/Region.hpp>

#include <memory>
#include <vector>
#include <optional>

namespace Hyprlooks {

class IRenderCommand {
  public:
    enum class ePriority : uint8_t {
        SHADOW    = 0,
        BACKGROUND = 1,
        BORDER    = 2,
        TEXTURE   = 3,
        GLOW      = 4,
        OVERLAY   = 5,
    };

    virtual ~IRenderCommand() = default;

    virtual UP<IPassElement>          produceElement(float monitorScale) = 0;
    virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const = 0;
    virtual ePriority                 priority() const                                      = 0;
    virtual bool                      isExpensive() const                                    = 0;
    virtual const char*               name() const                                           = 0;
};

}
