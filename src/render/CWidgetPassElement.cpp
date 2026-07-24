#include "CWidgetPassElement.hpp"

namespace Hyprlooks {

    CWidgetPassElement::CWidgetPassElement(CRenderCommandQueue&& queue, float scale) : m_queue(std::move(queue)), m_scale(scale) {
        m_needsBlur   = m_queue.hasExpensive();
        m_boundingBox = m_queue.totalBoundingBox(m_scale);
        ;
    }

    std::vector<UP<IPassElement>> CWidgetPassElement::draw() {
        return m_queue.produceAll(m_scale);
    }

    bool CWidgetPassElement::needsLiveBlur() {
        return m_needsBlur;
    }

    bool CWidgetPassElement::needsPrecomputeBlur() {
        return false;
    }

    std::optional<CBox> CWidgetPassElement::boundingBox() {
        return m_boundingBox;
    }

    CRegion CWidgetPassElement::opaqueRegion() {
        return {};
    }

    const char* CWidgetPassElement::passName() {
        return "CWidgetPassElement";
    }

    ePassElementType CWidgetPassElement::type() {
        return EK_CUSTOM;
    }

}
