#pragma once

#include "../core/SWidgetStyle.hpp"
#include "../core/IWidget.hpp"
#include "../theme/CThemeDefaults.hpp"

#include <string>

namespace Hyprlooks {

    class CTheme {
      public:
        CTheme()  = default;
        ~CTheme() = default;

        void         loadDefaults();

        SWidgetStyle resolve(eWidgetType type, const SWidgetStyle& overrides) const;
        SWidgetStyle baseStyle(eWidgetType type) const;

        void         setAccentColor(const CHyprColor& c);
        CHyprColor   accentColor() const;

      private:
        CHyprColor m_accent = CHyprColor{0.2F, 0.5F, 0.9F, 1.F};
    };

    UP<CTheme>& theme();

}
