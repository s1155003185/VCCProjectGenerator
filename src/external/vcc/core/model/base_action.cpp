#include "base_action.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "exception_macro.hpp"
#include "i_result.hpp"
#include "log_service.hpp"
#include "operation_result.hpp"
#include "string_helper.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    std::shared_ptr<LogConfig> BaseAction::getLogConfig() const
    {
        return _LogConfig;
    }

    void BaseAction::setLogConfig(std::shared_ptr<LogConfig> logConfig) const
    {
        _LogConfig = logConfig;
    }

    void BaseAction::logRedoStart() const
    { 
        TRY
            std::wstring message = getRedoMessageStart();
            if (!isBlank(message))
                LogService::logInfo(_LogConfig.get(), L"", message);
        CATCH
    }
    
    void BaseAction::logRedoComplete() const
    { 
        TRY
            std::wstring message = getRedoMessageComplete();
            if (!isBlank(message))
                LogService::logInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    void BaseAction::logUndoStart() const
    {
        TRY
            std::wstring message = getUndoMessageStart();
            if (!isBlank(message))
                LogService::logInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    void BaseAction::logUndoComplete() const
    {
        TRY
            std::wstring message = getUndoMessageComplete();
            if (!isBlank(message))
                LogService::logInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    // No clone Method for Action
    std::shared_ptr<IObject> BaseAction::clone() const
    {
        assert(false);
        return nullptr;
    }
    
    size_t BaseAction::getSeqNo() const
    { 
        return _SeqNo;
    }

    void BaseAction::setSeqNo(const size_t &seqNo) const 
    { 
        _SeqNo = seqNo; 
    }

    std::shared_ptr<IResult> BaseAction::Redo() 
    {
        TRY
            logRedoStart();
            auto result = onRedo();
            logRedoComplete();
            return result;
        CATCH_RETURN_RESULT(vcc::OperationResult)
        return std::make_shared<OperationResult>();
    }

    std::shared_ptr<IResult> BaseAction::Undo()
    {
        TRY
            logUndoStart();
            auto result = onUndo();
            logUndoComplete();
            return result;
        CATCH_RETURN_RESULT(vcc::OperationResult)
        return std::make_shared<OperationResult>();
    }
}
