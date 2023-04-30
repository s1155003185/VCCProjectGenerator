#pragma once
#include "base_manager.hpp"

namespace vcc
{
    class GitManager : public BaseManager
    {
        public:
            GitManager() : BaseManager(ManagerType::Git) {}
            ~GitManager() {}
    }
}
