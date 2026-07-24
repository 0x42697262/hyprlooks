#include "CDrawGlowCommand.hpp"

#include <src/render/OpenGL.hpp>

namespace Hyprlooks {

    CDrawGlowCommand::CDrawGlowCommand(SParams&& params) : m_params(std::move(params)) {
        ;
    }

    UP<IPassElement> CDrawGlowCommand::produceElement(float monitorScale) {
        return nullptr;
    }

    std::optional<Hyprutils::Math::CBox> CDrawGlowCommand::boundingBox(float monitorScale) const {
        auto box = m_params.box;
        return box.scale(monitorScale).round();
    }

    IRenderCommand::ePriority CDrawGlowCommand::priority() const {
        return ePriority::GLOW;
    }

    bool CDrawGlowCommand::isExpensive() const {
        return true;
    }

    const char* CDrawGlowCommand::name() const {
        return "CDrawGlowCommand";
    }

}
