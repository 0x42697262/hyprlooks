#pragma once

#include "IRenderCommand.hpp"

#include <vector>
#include <algorithm>

namespace Hyprlooks {

    class CRenderCommandQueue {
      public:
        CRenderCommandQueue()  = default;
        ~CRenderCommandQueue() = default;

        CRenderCommandQueue(const CRenderCommandQueue&)                       = delete;
        CRenderCommandQueue& operator=(const CRenderCommandQueue&)            = delete;
        CRenderCommandQueue(CRenderCommandQueue&&)                            = default;
        CRenderCommandQueue&                 operator=(CRenderCommandQueue&&) = default;

        void                                 add(UP<IRenderCommand> cmd);
        void                                 sortByPriority();
        void                                 filterExpensive(bool allowExpensive);
        void                                 clear();

        size_t                               size() const;
        bool                                 empty() const;
        bool                                 hasExpensive() const;

        std::vector<UP<IPassElement>>        produceAll(float monitorScale);
        std::optional<Hyprutils::Math::CBox> totalBoundingBox(float monitorScale) const;

      private:
        std::vector<UP<IRenderCommand>> m_commands;
    };

}
