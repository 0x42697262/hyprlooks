#include "CInputCommandQueue.hpp"

namespace Hyprlooks {

    void CInputCommandQueue::enqueue(UP<IInputCommand> cmd) {
        if (cmd)
            m_commands.push_back(std::move(cmd));
    }

    void CInputCommandQueue::flush() {
        for (auto& cmd : m_commands)
            cmd->execute();
        m_commands.clear();
    }

    void CInputCommandQueue::clear() {
        m_commands.clear();
    }

    size_t CInputCommandQueue::size() const {
        return m_commands.size();
    }

    bool CInputCommandQueue::empty() const {
        return m_commands.empty();
    }

}
