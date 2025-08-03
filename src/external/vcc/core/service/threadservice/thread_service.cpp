#include "thread_service.hpp"

#include <thread>

namespace vcc
{
    void ThreadService::ExecuteThread(std::shared_ptr<Thread> thread)
    {
        thread->execute();
    }

    void ThreadService::join(std::shared_ptr<Thread> thread)
    {
        std::thread t(ExecuteThread, thread);
        t.join();
    }
    
    void ThreadService::detach(std::shared_ptr<Thread> thread)
    {
        std::thread t(ExecuteThread, thread);
        t.detach();        
    }
};