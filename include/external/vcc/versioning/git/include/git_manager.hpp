#pragma once

#include <memory>

#include "base_manager.hpp"

namespace vcc
{
    class GitManager : public BaseManager<GitManager>
    {
        public:
            GitManager(std::shared_ptr<LogConfig> logProperty) : BaseManager(logProperty, ManagerType::Git) {}
            ~GitManager() {}
    };
}
