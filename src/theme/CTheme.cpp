#include "CTheme.hpp"

namespace Hyprlooks {

void CTheme::loadDefaults() {
    m_accent = CThemeDefaults::accentColor();
}

SWidgetStyle CTheme::baseStyle(eWidgetType type) const {
    switch (type) {
        case eWidgetType::BAR: return CThemeDefaults::barStyle();
        case eWidgetType::CONTAINER: return CThemeDefaults::containerStyle();
        case eWidgetType::LABEL: return CThemeDefaults::labelStyle();
        case eWidgetType::BOX: return CThemeDefaults::boxStyle();
        case eWidgetType::BUTTON: return CThemeDefaults::buttonStyle();
        case eWidgetType::IMAGE: return CThemeDefaults::imageStyle();
        default: return {};
    }
}

SWidgetStyle CTheme::resolve(eWidgetType type, const SWidgetStyle& overrides) const {
    return baseStyle(type).merge(overrides);
}

void CTheme::setAccentColor(const CHyprColor& c) {
    m_accent = c;
}

CHyprColor CTheme::accentColor() const {
    return m_accent;
}

UP<CTheme>& theme() {
    static UP<CTheme> t = makeUnique<CTheme>();
    return t;
}

}
