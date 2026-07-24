#include "CDrawBorderCommand.hpp"

namespace Hyprlooks {

    CDrawBorderCommand::CDrawBorderCommand(SParams&& params) : m_params(std::move(params)) {
        ;
    }

    UP<IPassElement> CDrawBorderCommand::produceElement(float monitorScale) {
        auto box = m_params.box;
        box.scale(monitorScale).round();

        CBorderPassElement::SBorderData data;
        data.box = box;
        data.grad1.m_colors.push_back(m_params.color);
        data.a             = m_params.alpha;
        data.round         = m_params.round;
        data.roundingPower = m_params.roundingPower;
        data.borderSize    = sc<int>(m_params.width * monitorScale);
        data.outerRound    = m_params.outerRound;

        return makeUnique<CBorderPassElement>(data);
    }

    std::optional<Hyprutils::Math::CBox> CDrawBorderCommand::boundingBox(float monitorScale) const {
        auto box = m_params.box;
        return box.scale(monitorScale).round();
    }

    IRenderCommand::ePriority CDrawBorderCommand::priority() const {
        return ePriority::BORDER;
    }

    bool CDrawBorderCommand::isExpensive() const {
        return false;
    }

    const char* CDrawBorderCommand::name() const {
        return "CDrawBorderCommand";
    }

}
