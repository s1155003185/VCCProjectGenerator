#pragma once

#include "base_manager.hpp"
#include "memory_macro.hpp"
#include "process_state.hpp"
#include "thread_management_mode.hpp"

#include <vector>

namespace vcc
{
    class Thread;

    enum class ThreadManagerTerminateMode
    {
        Immediately,
        Force,
        Wait
    };

    class ThreadManager : public BaseManager
    {
        GETSET(ThreadManagementMode, ThreadManagementMode, ThreadManagementMode::Detach)
        GETSET(ProcessState, State, ProcessState::Idle)
        GETSET(int64_t, MaxThreadPoolSize, 10)
        GETSET(ThreadManagerTerminateMode, TerminateMode, ThreadManagerTerminateMode::Wait)
    
    protected:
        mutable std::vector<std::shared_ptr<Thread>> _Threads;
        mutable std::vector<std::shared_ptr<Thread>> _ActiveThreads;

    public:
        ThreadManager(std::shared_ptr<LogConfig> logConfig) : BaseManager(logConfig) {}
        virtual ~ThreadManager();
        
        std::vector<std::shared_ptr<Thread>> &GetThreads() const;
        std::vector<std::shared_ptr<Thread>> &GetActiveThreads() const;

        // Add to Queue Tail
        void Queue(std::shared_ptr<Thread> thread) const;
        
        // Add to Queue First Piror
        void Urgent(std::shared_ptr<Thread> thread) const;

        // Alert Manager to do work
        void Trigger() const;

        // Execute Immediately
        void Join(std::shared_ptr<Thread> thread) const;
        
        // Execute Immediately
        void Detach(std::shared_ptr<Thread> thread) const;

        // Working State
        bool IsIdle() const;
        void Suspend() const;
        void Resume() const;
        void Stop() const;
        
        // Clear Waiting Thread
        void ClearWaitingThread() const;
    };
}
