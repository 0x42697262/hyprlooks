#pragma once

#include "../CLabel.hpp"

#include <src/managers/eventLoop/EventLoopTimer.hpp>

#include <string>

namespace Hyprlooks {

    class CClock : public CLabel {
      public:
        CClock();
        ~CClock() override;

        void                setFormat(const std::string& fmt);
        void                setIntervalMs(uint32_t ms);

        virtual eWidgetType type() const override;
        virtual const char* typeName() const override;

        void                tick();

      private:
        void                updateText();
        void                armTimer();

        std::string         m_format     = "%H:%M";
        uint32_t            m_intervalMs = 1000;
        SP<CEventLoopTimer> m_timer;
    };

}
