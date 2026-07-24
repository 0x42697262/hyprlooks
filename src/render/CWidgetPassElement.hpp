#pragma once

#include <src/render/pass/PassElement.hpp>

#include "../core/CWidgetTree.hpp"
#include "../render/CRenderCommandQueue.hpp"

namespace Hyprlooks {

class CWidgetPassElement : public IPassElement {
  public:
    explicit CWidgetPassElement(CRenderCommandQueue&& queue);
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
    bool                m_needsBlur = false;
};

}
