#pragma once
#include "base_service.hpp"
#include "thread.hpp"
#include "log_config.hpp"

namespace vcc
{
    class ThreadService : public BaseService
    {
        private:
            static void ExecuteThread(std::shared_ptr<Thread> thread);

        public:
            ThreadService() : BaseService() {}            
            virtual ~ThreadService() {}

            static void Join(std::shared_ptr<Thread> thread);
            static void Detach(std::shared_ptr<Thread> thread);
    };
}