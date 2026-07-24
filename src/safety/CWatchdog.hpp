#pragma once

#include <chrono>
#include <string>

namespace Hyprlooks {

    class CWatchdog {
      public:
        using Clock = std::chrono::steady_clock;

        struct STimeBudget {
            Clock::duration budget;
            Clock::duration elapsed;
            bool            exceeded() const;
        };

        CWatchdog()  = default;
        ~CWatchdog() = default;

        void                             beginRender();
        void                             endRender();
        bool                             renderExceeded() const;

        void                             beginInput();
        void                             endInput();
        bool                             inputExceeded() const;

        static constexpr Clock::duration RENDER_BUDGET = std::chrono::microseconds(5000);
        static constexpr Clock::duration INPUT_BUDGET  = std::chrono::microseconds(2000);

      private:
        STimeBudget       m_render{RENDER_BUDGET, {}};
        STimeBudget       m_input{INPUT_BUDGET, {}};
        Clock::time_point m_renderStart;
        Clock::time_point m_inputStart;
    };

    inline bool CWatchdog::STimeBudget::exceeded() const {
        return elapsed > budget;
    }

}
