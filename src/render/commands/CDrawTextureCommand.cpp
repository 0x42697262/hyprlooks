#include "CDrawTextureCommand.hpp"

namespace Hyprlooks {

    CDrawTextureCommand::CDrawTextureCommand(SParams&& params) : m_params(std::move(params)) {
        ;
    }

    UP<IPassElement> CDrawTextureCommand::produceElement(float monitorScale) {
        if (!m_params.tex || !m_params.tex->ok())
            return nullptr;

        CTexPassElement::SRenderData data;
        data.tex = m_params.tex;
        data.box = m_params.box;
        data.box.scale(monitorScale).round();
        data.a             = m_params.alpha;
        data.round         = m_params.round;
        data.roundingPower = m_params.roundingPower;

        if (m_params.clipBox.w > 0 && m_params.clipBox.h > 0) {
            data.clipBox = m_params.clipBox;
            data.clipBox.scale(monitorScale).round();
        }

        return makeUnique<CTexPassElement>(std::move(data));
    }

    std::optional<Hyprutils::Math::CBox> CDrawTextureCommand::boundingBox(float monitorScale) const {
        if (!m_params.tex || !m_params.tex->ok())
            return std::nullopt;
        auto box = m_params.box;
        return box.scale(monitorScale).round();
    }

    IRenderCommand::ePriority CDrawTextureCommand::priority() const {
        return ePriority::TEXTURE;
    }

    bool CDrawTextureCommand::isExpensive() const {
        return false;
    }

    const char* CDrawTextureCommand::name() const {
        return "CDrawTextureCommand";
    }

    void CDrawTextureCommand::setClip(const Hyprutils::Math::CBox& clip) {
        m_params.clipBox = clip;
    }

}
