#pragma once

#include "ILayoutStrategy.hpp"

namespace Hyprlooks {

struct SFlexOptions {
    eFlexDirection direction    = eFlexDirection::ROW;
    eFlexJustify   justify      = eFlexJustify::START;
    eFlexAlign     align        = eFlexAlign::CENTER;
    float          mainGap      = 0.F;
    float          crossGap     = 0.F;
    Hyprutils::Math::Vector2D padding      = {0.F, 0.F};
};

class CFlexLayout : public ILayoutStrategy {
  public:
    explicit CFlexLayout(SFlexOptions opts);

    virtual void        layout(const Hyprutils::Math::CBox& container, std::vector<IWidget*>& children) override;
    virtual const char* name() const override;

  private:
    SFlexOptions m_opts;
};

}
