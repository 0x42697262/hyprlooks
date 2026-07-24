#pragma once

#include <src/defines.hpp>

#include <hyprutils/memory/UniquePtr.hpp>

#include <unordered_map>

namespace Hyprlooks {

    enum class eReserveEdge : uint8_t {
        TOP    = 0,
        BOTTOM = 1,
        LEFT   = 2,
        RIGHT  = 3,
    };

    // Reserves monitor area for in-compositor surfaces (bars, docks) so tiled
    // windows do not get covered — the plugin equivalent of a layer-shell
    // exclusive zone.
    //
    // Hyprland recomputes the layer-surface (LS) reserved slot on every layer
    // arrange, which would wipe anything we wrote there. Instead we own the
    // ERROR_BAR dynamic slot: Hyprland only touches it for its config-error
    // overlay, and CReservedArea::calculate() sums all slots, so our reservation
    // persists across layer re-arranges without accumulating.
    class CHyprlandReservation {
      public:
        CHyprlandReservation()  = default;
        ~CHyprlandReservation() = default;

        // Accumulates `amount` px on `edge` for this monitor and re-applies.
        void reserveEdge(PHLMONITOR mon, eReserveEdge edge, double amount);
        // Drops our reservation for this monitor and rebuilds the clean LS area.
        void clear(PHLMONITOR mon);
        void clearAll();

      private:
        struct SEdges {
            double top    = 0.0;
            double right  = 0.0;
            double bottom = 0.0;
            double left   = 0.0;
        };

        void                                 apply(PHLMONITOR mon);

        std::unordered_map<MONITORID, SEdges> m_reserved;
    };

    UP<CHyprlandReservation>& reservation();

}
