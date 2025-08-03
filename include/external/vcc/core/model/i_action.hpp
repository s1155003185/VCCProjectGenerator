#pragma once

#include <memory>
#include <string>

#include "i_result.hpp"
#include "log_config.hpp"

namespace vcc
{
    class IAction
    {
    protected:
        IAction() {}
        virtual ~IAction() {}

        virtual std::shared_ptr<IResult> onRedo() = 0;
        virtual std::shared_ptr<IResult> onUndo() = 0;

        virtual std::wstring getRedoMessageStart() const = 0;
        virtual std::wstring getRedoMessageComplete() const = 0;
        virtual std::wstring getUndoMessageStart() const = 0;
        virtual std::wstring getUndoMessageComplete() const = 0;

    public:
        // Log
        virtual std::shared_ptr<LogConfig> getLogConfig() const = 0;
        virtual void setLogConfig(std::shared_ptr<LogConfig> logConfig) const = 0;
        
        virtual void setSeqNo(const size_t &seqNo) const = 0;
        virtual size_t getSeqNo() const = 0;
        
        virtual std::shared_ptr<IResult> Redo() = 0;
        virtual std::shared_ptr<IResult> Undo() = 0;
    };
}
