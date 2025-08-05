#include "thread_manager.hpp"

#include <assert.h>

#include "exception_macro.hpp"
#include "terminal_service.hpp"
#include "time_helper.hpp"
#include "thread.hpp"
#include "thread_service.hpp"

namespace vcc
{
    ThreadManager::~ThreadManager()
    {
        stop();
    }

    std::vector<std::shared_ptr<Thread>> &ThreadManager::getThreads() const
    {
        return _Threads;
    }

    std::vector<std::shared_ptr<Thread>> &ThreadManager::getActiveThreads() const
    {
        return _ActiveThreads;
    }

    void ThreadManager::queue(std::shared_ptr<Thread> thread) const
    {
        TRY
            _Threads.push_back(thread);
            trigger();
        CATCH
    }
    
    void ThreadManager::urgent(std::shared_ptr<Thread> thread) const
    {
        TRY
            _Threads.insert(_Threads.begin(), thread);
            trigger();
        CATCH
    }

    void ThreadManager::trigger() const
    {
        TRY
            // cannot use lock, or it will be dead lock
            // std::unique_lock lock(*_Mutex);
            switch (_State)
            {
            case ProcessState::Suspend:
            case ProcessState::Stop:
                return;            
            default:
                break;
            }
            if (!_ActiveThreads.empty()) {
                // remove all terminated Threads
                _ActiveThreads.erase(
                    std::remove_if(_ActiveThreads.begin(), _ActiveThreads.end(), 
                    [](std::shared_ptr<Thread> thread) {
                        return thread->getState() == ProcessState::Complete
                        || thread->getState() == ProcessState::Stop;
                    }));
            }
            if (!_Threads.empty() && _MaxThreadPoolSize > 0 && _ActiveThreads.size() < (size_t)_MaxThreadPoolSize) {
                auto firstElement = _Threads.front();
                firstElement->setManager(this);
                firstElement->setState(ProcessState::Idle);
                _ActiveThreads.push_back(firstElement);
                _Threads.erase(_Threads.begin());
                switch (_ThreadManagementMode)
                {
                case ThreadManagementMode::Detach:
                    ThreadService::detach(firstElement);
                    break;
                case ThreadManagementMode::Join:
                    ThreadService::join(firstElement);
                    break;
                default:
                    assert(false);
                    break;
                }
            }
            _State = this->_ActiveThreads.empty() ? ProcessState::Idle : ProcessState::Busy;
        CATCH
    }

    void ThreadManager::join(std::shared_ptr<Thread> thread) const
    {
        TRY
            ThreadService::join(thread);
        CATCH        
    }
    
    void ThreadManager::detach(std::shared_ptr<Thread> thread) const
    {
        TRY
            ThreadService::detach(thread);
        CATCH 
    }

    bool ThreadManager::isIdle() const
    {
        return _State == ProcessState::Idle || _State == ProcessState::Complete || _State == ProcessState::Stop;
    }

    void ThreadManager::suspend() const
    {
        TRY
            _State = ProcessState::Suspend;
        CATCH        
    }

    void ThreadManager::resume() const
    {
        TRY
            _State = ProcessState::Idle;
            trigger();
        CATCH
    }

    void ThreadManager::stop() const
    {
        TRY
            clearWaitingThread();
            for (auto &thread : _ActiveThreads)
                thread->setState(ProcessState::Stop);
            
            // Remove Complete thread
            trigger();
            if (_ActiveThreads.empty())
                return;

            if (_TerminateMode != ThreadManagerTerminateMode::Immediately) {
                if (_TerminateMode == ThreadManagerTerminateMode::Force) {
                    for (auto &thread : _ActiveThreads) {
                        TRY
                        #ifdef _WIN32
                            TerminalService::execute(_LogConfig.get(), L"Thread", L"taskkill /PID " + thread->getPid() + L" /F");
                        #else
                            TerminalService::execute(_LogConfig.get(), L"Thread", L"kill " + thread->getPid());
                        #endif
                        // wait 1s
                        Sleep(1000);
                        CATCH_SLIENT
                    }
                    // Remove Complete thread
                    trigger();
                } else if (_TerminateMode == ThreadManagerTerminateMode::Wait) {
                    auto tmpThread = std::make_shared<Thread>(_LogConfig, L"ThreadTerminate", L"Start", L"Complete",
                        [this](const Thread * /*thread*/) {
                        while (true) {
                            this->trigger();
                            if (this->getActiveThreads().empty())
                                break;
                            Sleep(500); // wait 0.5s
                        }
                    });
                    ThreadService::join(tmpThread);
                }
            }
            _ActiveThreads.clear();
            _State = ProcessState::Stop;
        CATCH
    }
    
    void ThreadManager::clearWaitingThread() const
    {
        TRY
            _Threads.clear();
        CATCH
    }
};