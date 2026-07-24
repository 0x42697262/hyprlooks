#pragma once

#include "IWidget.hpp"

#include <src/defines.hpp>

#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/memory/WeakPtr.hpp>
#include <hyprutils/memory/UniquePtr.hpp>
#include <hyprutils/math/Box.hpp>

#include <vector>
#include <unordered_map>
#include <string>

namespace Hyprlooks {

    class CRenderCommandQueue;

    class CWidgetTree {
      public:
        CWidgetTree()  = default;
        ~CWidgetTree() = default;

        void                  setMonitor(PHLMONITOR pMonitor);
        PHLMONITOR            monitor() const;

        void                  setRoot(UP<IWidget> root);
        IWidget*              root() const;

        void                  markAllDirty();
        void                  collectDirtyWidgets(std::vector<IWidget*>& out);

        void                  relayout();
        void                  collectRenderCommands(CRenderCommandQueue& queue, float monitorScale);

        Hyprutils::Math::CBox lastDamageBox() const;
        void                  setLastDamageBox(const Hyprutils::Math::CBox& box);

      private:
        void                  collectDirtyRecursive(IWidget* w, std::vector<IWidget*>& out);
        void                  relayoutRecursive(IWidget* w, const Hyprutils::Math::CBox& available);
        void                  collectRenderCommandsRecursive(IWidget* w, CRenderCommandQueue& queue, float monitorScale);

        UP<IWidget>           m_root;
        PHLMONITOR            m_monitor = nullptr;
        Hyprutils::Math::CBox m_lastDamage;
        bool                  m_needsRelayout = true;
    };

    UP<CWidgetTree>& widgetTreeForMonitor(PHLMONITOR pMonitor);
    void             removeWidgetTreeForMonitor(PHLMONITOR pMonitor);

}
