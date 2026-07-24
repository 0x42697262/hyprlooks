#include "CDrawShadowCommand.hpp"

#include <src/render/OpenGL.hpp>

namespace Hyprlooks {

CDrawShadowCommand::CDrawShadowCommand(SParams&& params) : m_params(std::move(params)) {
    ;
}

UP<IPassElement> CDrawShadowCommand::produceElement(float monitorScale) {
    return nullptr;
}

std::optional<Hyprutils::Math::CBox> CDrawShadowCommand::boundingBox(float monitorScale) const {
    auto  box = m_params.box;
    float r    = m_params.range * monitorScale;
    box.x     -= r;
    box.y     -= r;
    box.w     += r * 2.F;
    box.h     += r * 2.F;
    return box.round();
}

IRenderCommand::ePriority CDrawShadowCommand::priority() const {
    return ePriority::SHADOW;
}

bool CDrawShadowCommand::isExpensive() const {
    return true;
}

const char* CDrawShadowCommand::name() const {
    return "CDrawShadowCommand";
}

}
