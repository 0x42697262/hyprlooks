#pragma once

#include "CLabel.hpp"

#include "../state/IWidgetState.hpp"

#include <hyprutils/memory/WeakPtr.hpp>
#include <functional>
#include <string>

namespace Hyprlooks {

class CButton : public IWidget {
  public:
    using ClickCallback = std::function<void()>;

    CButton();
    ~CButton() override = default;

    void                   setText(const std::string& text);
    void                   setIcon(const std::string& icon);
    void                   onClick(ClickCallback cb);

    virtual eWidgetType    type() const override;
    virtual const char*    typeName() const override;
    virtual bool           interactive() const override;
    virtual void           layout(const Hyprutils::Math::CBox& available) override;
    virtual RenderCommandList buildRenderCommands(float monitorScale) override;

    void                   fireClick();

  private:
    void                   applyStateStyle();

    CLabel                 m_label;
    ClickCallback          m_clickCallback;
    bool                   m_pressed = false;
};

}
