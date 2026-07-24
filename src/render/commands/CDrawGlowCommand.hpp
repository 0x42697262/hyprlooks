#pragma once

#include "../IRenderCommand.hpp"

#include <src/helpers/Color.hpp>

#include <hyprutils/math/Box.hpp>

namespace Hyprlooks {

    class CDrawGlowCommand : public IRenderCommand {
      public:
        struct SParams {
            Hyprutils::Math::CBox box;
            int                   round         = 0;
            float                 roundingPower = 2.F;
            float                 range         = 10.F;
            int                   glowPower     = 2;
            CHyprColor            color         = CHyprColor{0.F, 0.F, 0.F, 0.5F};
            float                 alpha         = 1.F;
        };

        explicit CDrawGlowCommand(SParams&& params);

        virtual UP<IPassElement>                     produceElement(float monitorScale) override;
        virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const override;
        virtual ePriority                            priority() const override;
        virtual bool                                 isExpensive() const override;
        virtual const char*                          name() const override;

      private:
        SParams m_params;
    };

}
