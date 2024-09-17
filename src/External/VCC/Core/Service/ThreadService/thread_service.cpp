#include "thread_service.hpp"

#include <thread>

namespace vcc
{
    void ThreadService::ExecuteThread(std::shared_ptr<Thread> thread)
    {
        thread->Execute();
    }

    void ThreadService::Join(std::shared_ptr<Thread> thread)
    {
        std::thread t(ExecuteThread, thread);
        t.join();
    }
    
    void ThreadService::Detach(std::shared_ptr<Thread> thread)
    {
        std::thread t(ExecuteThread, thread);
        t.detach();        
    }
};