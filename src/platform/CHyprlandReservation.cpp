#include "CHyprlandReservation.hpp"

#include "../safety/CCrashIsolation.hpp"

#include <src/Compositor.hpp>
#include <src/render/Renderer.hpp>
#include <src/layout/LayoutManager.hpp>
#include <src/helpers/Monitor.hpp>
#include <src/desktop/reserved/ReservedArea.hpp>

namespace Hyprlooks {

    void CHyprlandReservation::reserveEdge(PHLMONITOR mon, eReserveEdge edge, double amount) {
        if (!mon || amount <= 0.0)
            return;

        auto& e = m_reserved[mon->m_id];
        switch (edge) {
            case eReserveEdge::TOP: e.top += amount; break;
            case eReserveEdge::BOTTOM: e.bottom += amount; break;
            case eReserveEdge::LEFT: e.left += amount; break;
            case eReserveEdge::RIGHT: e.right += amount; break;
        }

        apply(mon);
    }

    void CHyprlandReservation::apply(PHLMONITOR mon) {
        if (!mon)
            return;

        CCrashIsolation::guard("reservation.apply", [&]() {
            const auto& e = m_reserved[mon->m_id];

            // reset + add == set, so re-applying never accumulates.
            mon->m_reservedArea.resetType(Desktop::RESERVED_DYNAMIC_TYPE_ERROR_BAR);
            mon->m_reservedArea.addType(Desktop::RESERVED_DYNAMIC_TYPE_ERROR_BAR, Vector2D{e.left, e.top}, Vector2D{e.right, e.bottom});

            if (g_layoutManager)
                g_layoutManager->invalidateMonitorGeometries(mon);
        });
    }

    void CHyprlandReservation::clear(PHLMONITOR mon) {
        if (!mon)
            return;

        CCrashIsolation::guard("reservation.clear", [&]() {
            m_reserved.erase(mon->m_id);

            mon->m_reservedArea.resetType(Desktop::RESERVED_DYNAMIC_TYPE_ERROR_BAR);

            // rebuild the LS reserved area cleanly from real layer surfaces
            if (g_pHyprRenderer)
                g_pHyprRenderer->arrangeLayersForMonitor(mon->m_id);
            if (g_layoutManager)
                g_layoutManager->invalidateMonitorGeometries(mon);
        });
    }

    void CHyprlandReservation::clearAll() {
        if (g_pCompositor) {
            for (const auto& mon : g_pCompositor->m_monitors)
                clear(mon);
        }
        m_reserved.clear();
    }

    UP<CHyprlandReservation>& reservation() {
        static UP<CHyprlandReservation> r = makeUnique<CHyprlandReservation>();
        return r;
    }

}
