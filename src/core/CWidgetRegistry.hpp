#pragma once

#include "IWidget.hpp"

#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/memory/UniquePtr.hpp>

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace Hyprlooks {

    class CWidgetRegistry {
      public:
        using WidgetConstructor = std::function<UP<IWidget>()>;

        void                     registerType(const std::string& type, WidgetConstructor ctor);
        UP<IWidget>              create(const std::string& type);
        bool                     knows(const std::string& type) const;
        std::vector<std::string> registeredTypes() const;

      private:
        std::unordered_map<std::string, WidgetConstructor> m_constructors;
    };

    UP<CWidgetRegistry>& widgetRegistry();

}
