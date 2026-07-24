#pragma once

#include "../IRenderCommand.hpp"

#include <src/helpers/Color.hpp>
#include <src/render/pass/BorderPassElement.hpp>

#include <hyprutils/math/Box.hpp>

namespace Hyprlooks {

class CDrawBorderCommand : public IRenderCommand {
  public:
    struct SParams {
        Hyprutils::Math::CBox box;
        float                 width         = 1.F;
        CHyprColor            color         = CHyprColor{1.F, 1.F, 1.F, 1.F};
        int                   round         = 0;
        float                 roundingPower = 2.F;
        int                   outerRound    = -1;
        float                 alpha         = 1.F;
    };

    explicit CDrawBorderCommand(SParams&& params);

    virtual UP<IPassElement>          produceElement(float monitorScale) override;
    virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const override;
    virtual ePriority                 priority() const override;
    virtual bool                      isExpensive() const override;
    virtual const char*               name() const override;

  private:
    SParams m_params;
};

}
