#include "CDrawRectCommand.hpp"

namespace Hyprlooks {

CDrawRectCommand::CDrawRectCommand(SParams&& params) : m_params(std::move(params)) {
    ;
}

UP<IPassElement> CDrawRectCommand::produceElement(float monitorScale) {
    CRectPassElement::SRectData data;
    data.box           = m_params.box;
    data.box.scale(monitorScale).round();
    data.color         = m_params.color;
    data.round         = m_params.round;
    data.roundingPower = m_params.roundingPower;
    data.blur          = m_params.blur;
    data.blurA         = m_params.blurAlpha;

    if (m_params.clipBox.w > 0 && m_params.clipBox.h > 0) {
        data.clipBox = m_params.clipBox;
        data.clipBox.scale(monitorScale).round();
    }

    return makeUnique<CRectPassElement>(data);
}

std::optional<Hyprutils::Math::CBox> CDrawRectCommand::boundingBox(float monitorScale) const {
    auto box = m_params.box;
    return box.scale(monitorScale).round();
}

IRenderCommand::ePriority CDrawRectCommand::priority() const {
    return ePriority::BACKGROUND;
}

bool CDrawRectCommand::isExpensive() const {
    return m_params.blur;
}

const char* CDrawRectCommand::name() const {
    return "CDrawRectCommand";
}

}
