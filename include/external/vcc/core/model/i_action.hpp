#pragma once

#include <memory>
#include <string>

#include "log_config.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    class IAction
    {
    protected:
        IAction() {}
        virtual ~IAction() {}

        virtual void OnRedo() const = 0;
        virtual void OnUndo() const = 0;

        virtual std::wstring GetRedoMessageStart() const = 0;
        virtual std::wstring GetRedoMessageComplete() const = 0;
        virtual std::wstring GetUndoMessageStart() const = 0;
        virtual std::wstring GetUndoMessageComplete() const = 0;

    public:
        // Log
        virtual std::shared_ptr<LogConfig> GetLogConfig() const = 0;
        virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const = 0;
        
        // Thead
        virtual std::shared_ptr<ThreadManager> GetThreadManager() const = 0;
        virtual void SetThreadManager(std::shared_ptr<ThreadManager> threadManager) const = 0;
        
        virtual void SetSeqNo(const size_t &seqNo) const = 0;
        virtual size_t GetSeqNo() const = 0;
        
        virtual void Redo() const = 0;
        virtual void Undo() const = 0;
    };
}
