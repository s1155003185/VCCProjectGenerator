#pragma once

#include "base_manager.hpp"
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
        
        std::vector<std::shared_ptr<Thread>> &getThreads() const;
        std::vector<std::shared_ptr<Thread>> &getActiveThreads() const;

        // Add to Queue Tail
        void queue(std::shared_ptr<Thread> thread) const;
        
        // Add to Queue First Piror
        void urgent(std::shared_ptr<Thread> thread) const;

        // Alert Manager to do work
        void trigger() const;

        // Execute Immediately
        void join(std::shared_ptr<Thread> thread) const;
        
        // Execute Immediately
        void detach(std::shared_ptr<Thread> thread) const;

        // Working State
        bool isIdle() const;
        void suspend() const;
        void resume() const;
        void stop() const;
        
        // Clear Waiting Thread
        void clearWaitingThread() const;
    };
}
