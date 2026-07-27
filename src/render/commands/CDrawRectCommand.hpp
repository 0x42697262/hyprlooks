#pragma once

#include "../IRenderCommand.hpp"

#include <src/helpers/Color.hpp>
#include <src/render/pass/RectPassElement.hpp>

#include <hyprutils/math/Box.hpp>

namespace Hyprlooks {

    class CDrawRectCommand : public IRenderCommand {
      public:
        struct SParams {
            Hyprutils::Math::CBox box;
            CHyprColor            color;
            int                   round         = 0;
            float                 roundingPower = 2.F;
            bool                  blur          = false;
            float                 blurAlpha     = 1.F;
            Hyprutils::Math::CBox clipBox       = {};
        };

        explicit CDrawRectCommand(SParams&& params);

        virtual UP<IPassElement>                     produceElement(float monitorScale) override;
        virtual std::optional<Hyprutils::Math::CBox> boundingBox(float monitorScale) const override;
        virtual ePriority                            priority() const override;
        virtual bool                                 isExpensive() const override;
        virtual const char*                          name() const override;
        virtual void                                 setClip(const Hyprutils::Math::CBox& clip) override;

      private:
        SParams m_params;
    };

}
