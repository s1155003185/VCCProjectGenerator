#pragma once

#include <memory>

#include "base_manager.hpp"

namespace vcc
{
    class GitManager : public BaseManager
    {
        public:
            GitManager(std::shared_ptr<LogConfig> logConfig) : BaseManager(logConfig) {}
            ~GitManager() {}
    };
}
