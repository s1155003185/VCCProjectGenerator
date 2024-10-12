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
        Stop();
    }

    std::vector<std::shared_ptr<Thread>> &ThreadManager::GetThreads() const
    {
        return _Threads;
    }

    std::vector<std::shared_ptr<Thread>> &ThreadManager::GetActiveThreads() const
    {
        return _ActiveThreads;
    }

    void ThreadManager::Queue(std::shared_ptr<Thread> thread) const
    {
        TRY
            _Threads.push_back(thread);
            Trigger();
        CATCH
    }
    
    void ThreadManager::Urgent(std::shared_ptr<Thread> thread) const
    {
        TRY
            _Threads.insert(_Threads.begin(), thread);
            Trigger();
        CATCH
    }

    void ThreadManager::Trigger() const
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
                        return thread->GetState() == ProcessState::Complete
                        || thread->GetState() == ProcessState::Stop;
                    }));
            }
            if (!_Threads.empty() && _MaxThreadPoolSize > 0 && _ActiveThreads.size() < (size_t)_MaxThreadPoolSize) {
                auto firstElement = _Threads.front();
                firstElement->SetManager(this);
                firstElement->SetState(ProcessState::Idle);
                _ActiveThreads.push_back(firstElement);
                _Threads.erase(_Threads.begin());
                switch (_ThreadManagementMode)
                {
                case ThreadManagementMode::Detach:
                    ThreadService::Detach(firstElement);
                    break;
                case ThreadManagementMode::Join:
                    ThreadService::Join(firstElement);
                    break;
                default:
                    assert(false);
                    break;
                }
            }
            _State = this->_ActiveThreads.empty() ? ProcessState::Idle : ProcessState::Busy;
        CATCH
    }

    void ThreadManager::Join(std::shared_ptr<Thread> thread) const
    {
        TRY
            ThreadService::Join(thread);
        CATCH        
    }
    
    void ThreadManager::Detach(std::shared_ptr<Thread> thread) const
    {
        TRY
            ThreadService::Detach(thread);
        CATCH 
    }

    bool ThreadManager::IsIdle() const
    {
        return _State == ProcessState::Idle || _State == ProcessState::Complete || _State == ProcessState::Stop;
    }

    void ThreadManager::Suspend() const
    {
        TRY
            _State = ProcessState::Suspend;
        CATCH        
    }

    void ThreadManager::Resume() const
    {
        TRY
            _State = ProcessState::Idle;
            Trigger();
        CATCH
    }

    void ThreadManager::Stop() const
    {
        TRY
            ClearWaitingThread();
            for (auto &thread : _ActiveThreads)
                thread->SetState(ProcessState::Stop);
            
            // Remove Complete thread
            Trigger();
            if (_ActiveThreads.empty())
                return;

            if (_TerminateMode != ThreadManagerTerminateMode::Immediately) {
                if (_TerminateMode == ThreadManagerTerminateMode::Force) {
                    for (auto &thread : _ActiveThreads) {
                        TRY
                        #ifdef _WIN32
                            TerminalService::Execute(_LogConfig.get(), L"Thread", L"taskkill /PID " + thread->GetPid() + L" /F");
                        #else
                            TerminalService::Execute(_LogConfig.get(), L"Thread", L"kill " + thread->GetPid());
                        #endif
                        // wait 1s
                        Sleep(1000);
                        CATCH_SLIENT
                    }
                    // Remove Complete thread
                    Trigger();
                } else if (_TerminateMode == ThreadManagerTerminateMode::Wait) {
                    DECLARE_SPTR(Thread, tmpThread, _LogConfig, L"ThreadTerminate", L"Start", L"Complete", [this](const Thread * /*thread*/) {
                        while (true) {
                            this->Trigger();
                            if (this->GetActiveThreads().empty())
                                break;
                            Sleep(500); // wait 0.5s
                        }
                    });
                    ThreadService::Join(tmpThread);
                }
            }
            _ActiveThreads.clear();
            _State = ProcessState::Stop;
        CATCH
    }
    
    void ThreadManager::ClearWaitingThread() const
    {
        TRY
            _Threads.clear();
        CATCH
    }
};