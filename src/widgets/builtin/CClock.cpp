#include "CClock.hpp"
#include "../../theme/CThemeDefaults.hpp"
#include "../../platform/CHyprlandRenderer.hpp"

#include <src/managers/eventLoop/EventLoopManager.hpp>

#include <ctime>
#include <format>

namespace Hyprlooks {

    CClock::CClock() : CLabel() {
        m_style = CThemeDefaults::labelStyle();
        updateText();
        armTimer();
    }

    CClock::~CClock() {
        if (m_timer)
            m_timer->cancel();
    }

    void CClock::setFormat(const std::string& fmt) {
        if (m_format == fmt)
            return;
        m_format = fmt;
        updateText();
        markDirty();
    }

    void CClock::setIntervalMs(uint32_t ms) {
        m_intervalMs = std::max(1U, ms);
        armTimer();
    }

    eWidgetType CClock::type() const {
        return eWidgetType::CLOCK;
    }

    const char* CClock::typeName() const {
        return "CClock";
    }

    void CClock::tick() {
        updateText();
        markDirty();
        static CHyprlandRenderer renderer;
        renderer.scheduleAllFrames();
        armTimer();
    }

    void CClock::updateText() {
        auto  now   = std::time(nullptr);
        auto* tmVal = std::localtime(&now);
        char  buf[256];
        std::strftime(buf, sizeof(buf), m_format.c_str(), tmVal);
        setText(buf);
    }

    void CClock::armTimer() {
        if (!g_pEventLoopManager)
            return;

        if (m_timer)
            m_timer->cancel();

        m_timer = makeShared<CEventLoopTimer>(
            std::chrono::milliseconds(m_intervalMs),
            [](SP<CEventLoopTimer> self, void* data) {
                auto* clock = static_cast<CClock*>(data);
                if (clock)
                    clock->tick();
            },
            this);
        g_pEventLoopManager->addTimer(m_timer);
    }

}
