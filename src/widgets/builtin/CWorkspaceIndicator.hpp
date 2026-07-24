#pragma once

#include "../CContainer.hpp"
#include "../CButton.hpp"

#include <hyprutils/signal/Signal.hpp>

#include <string>
#include <functional>

namespace Hyprlooks {

    class CWorkspaceIndicator : public CContainer {
      public:
        CWorkspaceIndicator();
        ~CWorkspaceIndicator() override = default;

        void                setMonitorName(const std::string& name);
        void                setActiveBgColor(const CHyprColor& c);
        void                setInactiveFgColor(const CHyprColor& c);

        virtual eWidgetType type() const override;
        virtual const char* typeName() const override;

        void                rebuild();

      private:
        void                                   onWorkspaceActive();

        std::string                            m_monitorName;
        CHyprColor                             m_activeBgColor   = CHyprColor{0.2F, 0.5F, 0.9F, 1.F};
        CHyprColor                             m_inactiveFgColor = CHyprColor{0.5F, 0.5F, 0.55F, 1.F};

        Hyprutils::Signal::CHyprSignalListener m_workspaceListener;
    };

}
