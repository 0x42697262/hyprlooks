#pragma once

#include "SWidgetGeometry.hpp"
#include "SWidgetStyle.hpp"

#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/memory/WeakPtr.hpp>
#include <hyprutils/memory/UniquePtr.hpp>

#include <string>
#include <vector>
#include <functional>

namespace Hyprlooks {

    class IWidgetState;
    class IRenderCommand;
    class ILayoutStrategy;
    class CWidgetTree;

    enum class eWidgetType : uint8_t {
        UNKNOWN = 0,
        BAR,
        CONTAINER,
        PANEL,
        SCROLL,
        LABEL,
        BOX,
        BUTTON,
        IMAGE,
        WORKSPACE_INDICATOR,
        CLOCK,
        VOLUME,
    };

    class IWidget {
      public:
        using RenderCommandList = std::vector<UP<IRenderCommand>>;

        IWidget()          = default;
        virtual ~IWidget() = default;

        IWidget(const IWidget&)                                     = delete;
        IWidget(IWidget&&)                                          = delete;
        IWidget&                          operator=(const IWidget&) = delete;
        IWidget&                          operator=(IWidget&&)      = delete;

        virtual eWidgetType               type() const     = 0;
        virtual const char*               typeName() const = 0;

        virtual Hyprutils::Math::Vector2D measure() const = 0;
        virtual void                      layout(const Hyprutils::Math::CBox& available);
        virtual RenderCommandList         buildRenderCommands(float monitorScale) = 0;

        virtual bool                      hitTest(const Hyprutils::Math::Vector2D& localPos) const;
        virtual bool                      interactive() const;
        virtual void                      setState(UP<IWidgetState>&& state);
        IWidgetState*                     state() const;

        const SWidgetGeometry&            geometry() const;
        SWidgetGeometry&                  geometry();

        const SWidgetStyle&               style() const;
        virtual void                      setStyle(const SWidgetStyle& s);

        void                              setParent(IWidget* parent);
        IWidget*                          parent() const;
        void                              setSelf(WP<IWidget> self);
        WP<IWidget>                       self() const;

        void                              markDirty();
        bool                              isDirty() const;

        void                              setVisible(bool visible);
        bool                              isVisible() const;

        void                              setStyleDirty();
        bool                              isStyleDirty() const;

        void                              setId(const std::string& id);
        const std::string&                id() const;

      protected:
        SWidgetGeometry  m_geometry;
        SWidgetStyle     m_style;
        UP<IWidgetState> m_state;
        IWidget*         m_parent = nullptr;
        WP<IWidget>      m_self;
        std::string      m_id;
        bool             m_visible    = true;
        bool             m_dirty      = true;
        bool             m_styleDirty = true;
    };

}
