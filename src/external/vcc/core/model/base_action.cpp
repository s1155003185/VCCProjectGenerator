#include "base_action.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "exception_macro.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    std::shared_ptr<LogConfig> BaseAction::GetLogConfig() const
    {
        return _LogConfig;
    }

    void BaseAction::SetLogConfig(std::shared_ptr<LogConfig> logConfig) const
    {
        _LogConfig = logConfig;
    }

    std::shared_ptr<ThreadManager> BaseAction::GetThreadManager() const
    {
        return _ThreadManager;
    }

    void BaseAction::SetThreadManager(std::shared_ptr<ThreadManager> threadManager) const
    {
        _ThreadManager = threadManager;
    }

    void BaseAction::LogRedo() const
    { 
        TRY
            std::wstring message = GetRedoMessage();
            if (!IsBlank(message))
                LogService::LogInfo(_LogConfig.get(), L"", message);
        CATCH
    }
    
    void BaseAction::LogUndo() const
    {
        TRY
            std::wstring message = GetUndoMessage();
            if (!IsBlank(message))
                LogService::LogInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    // No Clone Method for Action
    std::shared_ptr<IObject> BaseAction::Clone() const
    {
        assert(false);
        return nullptr;
    }
    
    size_t BaseAction::GetSeqNo() const
    { 
        return _SeqNo;
    }
    
    void BaseAction::SetSeqNo(const size_t &seqNo) const 
    { 
        _SeqNo = seqNo; 
    }

    void BaseAction::Redo() const 
    {
        TRY
            OnRedo();
            LogRedo();
        CATCH
    }

    void BaseAction::Undo() const
    {
        TRY
            OnUndo();
            LogUndo();
        CATCH
    }
}
