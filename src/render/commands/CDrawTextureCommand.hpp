#pragma once

#include "../IRenderCommand.hpp"

#include <src/render/Texture.hpp>
#include <src/render/pass/TexPassElement.hpp>

#include <hyprutils/math/Box.hpp>

namespace Hyprlooks {

class CDrawTextureCommand : public IRenderCommand {
  public:
    struct SParams {
        SP<Render::ITexture>   tex;
        Hyprutils::Math::CBox  box;
        float                  alpha          = 1.F;
        int                    round          = 0;
        float                  roundingPower  = 2.F;
        Hyprutils::Math::CBox  clipBox        = {};
    };

    explicit CDrawTextureCommand(SParams&& params);

    virtual UP<IPassElement>          produceElement(float monitorScale) override;
    virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const override;
    virtual ePriority                 priority() const override;
    virtual bool                      isExpensive() const override;
    virtual const char*               name() const override;

  private:
    SParams m_params;
};

}
