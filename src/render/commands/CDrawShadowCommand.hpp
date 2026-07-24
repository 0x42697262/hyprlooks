#pragma once

#include "../IRenderCommand.hpp"

#include <src/helpers/Color.hpp>
#include <src/render/pass/ShadowPassElement.hpp>

#include <hyprutils/math/Box.hpp>

namespace Hyprlooks {

    class CDrawShadowCommand : public IRenderCommand {
      public:
        struct SParams {
            Hyprutils::Math::CBox box;
            int                   round         = 0;
            float                 roundingPower = 2.F;
            float                 range         = 20.F;
            CHyprColor            color         = CHyprColor{0.F, 0.F, 0.F, 0.4F};
            float                 alpha         = 1.F;
        };

        explicit CDrawShadowCommand(SParams&& params);

        virtual UP<IPassElement>                     produceElement(float monitorScale) override;
        virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const override;
        virtual ePriority                            priority() const override;
        virtual bool                                 isExpensive() const override;
        virtual const char*                          name() const override;

      private:
        SParams m_params;
    };

}
