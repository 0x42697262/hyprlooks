#pragma once

#include "CContainer.hpp"

#include <string>

namespace Hyprlooks {

    enum class eBarPosition : uint8_t {
        TOP    = 0,
        BOTTOM = 1,
        LEFT   = 2,
        RIGHT  = 3,
    };

    class CBar : public CContainer {
      public:
        CBar();
        ~CBar() override = default;

        void                              setMonitorName(const std::string& name);
        const std::string&                monitorName() const;

        void                              setPosition(eBarPosition pos);
        eBarPosition                      position() const;

        void                              setHeight(float h);
        float                             height() const;

        void                              setWidth(float w);
        float                             width() const;

        CContainer*                       leftSection();
        CContainer*                       centerSection();
        CContainer*                       rightSection();

        virtual eWidgetType               type() const override;
        virtual const char*               typeName() const override;
        virtual Hyprutils::Math::Vector2D measure() const override;
        virtual void                      layout(const Hyprutils::Math::CBox& available) override;
        virtual RenderCommandList         buildRenderCommands(float monitorScale) override;

      private:
        void           ensureSections();

        std::string    m_monitorName;
        eBarPosition   m_position = eBarPosition::TOP;
        float          m_height   = 44.F;
        float          m_width    = 0.F;

        UP<CContainer> m_left;
        UP<CContainer> m_center;
        UP<CContainer> m_right;

        bool           m_sectionsBuilt = false;
    };

}
