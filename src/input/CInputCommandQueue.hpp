#pragma once

#include "IInputCommand.hpp"

#include <vector>
#include <hyprutils/memory/UniquePtr.hpp>

namespace Hyprlooks {

class CInputCommandQueue {
  public:
    CInputCommandQueue()  = default;
    ~CInputCommandQueue() = default;

    void   enqueue(UP<IInputCommand> cmd);
    void   flush();
    void   clear();

    size_t size() const;
    bool   empty() const;

  private:
    std::vector<UP<IInputCommand>> m_commands;
};

}
