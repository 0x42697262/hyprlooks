#pragma once

#include <src/render/pass/PassElement.hpp>

#include "../core/CWidgetTree.hpp"
#include "../render/CRenderCommandQueue.hpp"

namespace Hyprlooks {

    class CWidgetPassElement : public IPassElement {
      public:
        CWidgetPassElement(CRenderCommandQueue&& queue, float scale);
        virtual ~CWidgetPassElement() = default;

        virtual std::vector<UP<IPassElement>> draw() override;
        virtual bool                          needsLiveBlur() override;
        virtual bool                          needsPrecomputeBlur() override;
        virtual std::optional<CBox>           boundingBox() override;
        virtual CRegion                       opaqueRegion() override;
        virtual const char*                   passName() override;
        virtual ePassElementType              type() override;

      private:
        CRenderCommandQueue m_queue;
        float               m_scale     = 1.F;
        bool                m_needsBlur = false;
        std::optional<CBox> m_boundingBox;
    };

}
