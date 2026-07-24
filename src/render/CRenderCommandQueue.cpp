#include "CRenderCommandQueue.hpp"

namespace Hyprlooks {

void CRenderCommandQueue::add(UP<IRenderCommand> cmd) {
    if (cmd)
        m_commands.push_back(std::move(cmd));
}

void CRenderCommandQueue::sortByPriority() {
    std::ranges::sort(m_commands, [](const UP<IRenderCommand>& a, const UP<IRenderCommand>& b) { return static_cast<int>(a->priority()) < static_cast<int>(b->priority()); });
}

void CRenderCommandQueue::filterExpensive(bool allowExpensive) {
    if (allowExpensive)
        return;
    std::erase_if(m_commands, [](const UP<IRenderCommand>& cmd) { return cmd->isExpensive(); });
}

void CRenderCommandQueue::clear() {
    m_commands.clear();
}

size_t CRenderCommandQueue::size() const {
    return m_commands.size();
}

bool CRenderCommandQueue::empty() const {
    return m_commands.empty();
}

std::vector<UP<IPassElement>> CRenderCommandQueue::produceAll(float monitorScale) {
    std::vector<UP<IPassElement>> result;
    result.reserve(m_commands.size());
    for (auto& cmd : m_commands) {
        auto el = cmd->produceElement(monitorScale);
        if (el)
            result.push_back(std::move(el));
    }
    return result;
}

std::optional<Hyprutils::Math::CBox> CRenderCommandQueue::totalBoundingBox(float monitorScale) const {
    if (m_commands.empty())
        return std::nullopt;

    std::optional<Hyprutils::Math::CBox> result;
    for (const auto& cmd : m_commands) {
        auto box = cmd->boundingBox(monitorScale);
        if (!box)
            continue;
        if (!result) {
            result = *box;
            continue;
        }
        auto& r     = *result;
        r.x         = std::min(r.x, box->x);
        r.y         = std::min(r.y, box->y);
        r.w         = std::max(r.x + r.w, box->x + box->w) - r.x;
        r.h         = std::max(r.y + r.h, box->y + box->h) - r.y;
    }
    return result;
}

}
