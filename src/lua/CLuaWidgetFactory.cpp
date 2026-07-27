#include "CLuaWidgetFactory.hpp"
#include "../core/CWidgetRegistry.hpp"
#include "../widgets/CBar.hpp"
#include "../widgets/CContainer.hpp"
#include "../widgets/CPanel.hpp"
#include "../widgets/CScroll.hpp"
#include "../widgets/CInput.hpp"
#include "../widgets/CLabel.hpp"
#include "../widgets/CBox.hpp"
#include "../widgets/CButton.hpp"
#include "../widgets/CImage.hpp"
#include "../widgets/builtin/CClock.hpp"
#include "../widgets/builtin/CWorkspaceIndicator.hpp"

namespace Hyprlooks {

    void CLuaWidgetFactory::registerBuiltinWidgets() {
        registerWidgetTypes();
    }

    void CLuaWidgetFactory::registerWidgetTypes() {
        widgetRegistry()->registerType("bar", [] { return makeUnique<CBar>(); });
        widgetRegistry()->registerType("container", [] { return makeUnique<CContainer>(); });
        widgetRegistry()->registerType("panel", [] { return makeUnique<CPanel>(); });
        widgetRegistry()->registerType("scroll", [] { return makeUnique<CScroll>(); });
        widgetRegistry()->registerType("input", [] { return makeUnique<CInput>(); });
        widgetRegistry()->registerType("label", [] { return makeUnique<CLabel>(); });
        widgetRegistry()->registerType("box", [] { return makeUnique<CBox>(); });
        widgetRegistry()->registerType("button", [] { return makeUnique<CButton>(); });
        widgetRegistry()->registerType("image", [] { return makeUnique<CImage>(); });
        widgetRegistry()->registerType("clock", [] { return makeUnique<CClock>(); });
        widgetRegistry()->registerType("workspaces", [] { return makeUnique<CWorkspaceIndicator>(); });
    }

    UP<CLuaWidgetFactory>& luaWidgetFactory() {
        static UP<CLuaWidgetFactory> factory = makeUnique<CLuaWidgetFactory>();
        return factory;
    }

}
