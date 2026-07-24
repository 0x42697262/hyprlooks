#include "CWidgetRegistry.hpp"

namespace Hyprlooks {

void CWidgetRegistry::registerType(const std::string& type, WidgetConstructor ctor) {
    m_constructors[type] = std::move(ctor);
}

UP<IWidget> CWidgetRegistry::create(const std::string& type) {
    auto it = m_constructors.find(type);
    if (it == m_constructors.end())
        return nullptr;
    return it->second();
}

bool CWidgetRegistry::knows(const std::string& type) const {
    return m_constructors.contains(type);
}

std::vector<std::string> CWidgetRegistry::registeredTypes() const {
    std::vector<std::string> result;
    result.reserve(m_constructors.size());
    for (const auto& [name, _] : m_constructors)
        result.push_back(name);
    return result;
}

UP<CWidgetRegistry>& widgetRegistry() {
    static UP<CWidgetRegistry> registry = makeUnique<CWidgetRegistry>();
    return registry;
}

}
