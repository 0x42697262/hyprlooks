#include "CWidgetPassElement.hpp"
#include "commands/CDrawRectCommand.hpp"

namespace Hyprlooks {

CWidgetPassElement::CWidgetPassElement(CRenderCommandQueue&& queue) : m_queue(std::move(queue)) {
    ;
}

std::vector<UP<IPassElement>> CWidgetPassElement::draw() {
    return m_queue.produceAll(1.F);
}

bool CWidgetPassElement::needsLiveBlur() {
    return m_needsBlur;
}

bool CWidgetPassElement::needsPrecomputeBlur() {
    return false;
}

std::optional<CBox> CWidgetPassElement::boundingBox() {
    return std::nullopt;
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
