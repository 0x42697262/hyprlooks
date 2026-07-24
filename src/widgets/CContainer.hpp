#pragma once

#include "../core/IWidget.hpp"
#include "../layout/ILayoutStrategy.hpp"

#include <vector>
#include <memory>

namespace Hyprlooks {

class CContainer : public IWidget {
  public:
    CContainer();
    ~CContainer() override = default;

    void                   addChild(UP<IWidget> child);
    void                   clearChildren();
    size_t                 childCount() const;

    void                   setLayoutStrategy(UP<ILayoutStrategy> strategy);
    ILayoutStrategy*       layoutStrategy() const;

    virtual eWidgetType    type() const override;
    virtual const char*    typeName() const override;
    virtual void           layout(const Hyprutils::Math::CBox& available) override;
    virtual RenderCommandList buildRenderCommands(float monitorScale) override;

  private:
    std::vector<UP<IWidget>>    m_children;
    UP<ILayoutStrategy>         m_layoutStrategy;
};

}
