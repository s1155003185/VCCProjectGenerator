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

    void BaseAction::LogRedoStart() const
    { 
        TRY
            std::wstring message = GetRedoMessageStart();
            if (!IsBlank(message))
                LogService::LogInfo(_LogConfig.get(), L"", message);
        CATCH
    }
    
    void BaseAction::LogRedoComplete() const
    { 
        TRY
            std::wstring message = GetRedoMessageComplete();
            if (!IsBlank(message))
                LogService::LogInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    void BaseAction::LogUndoStart() const
    {
        TRY
            std::wstring message = GetUndoMessageStart();
            if (!IsBlank(message))
                LogService::LogInfo(_LogConfig.get(), L"", message);
        CATCH
    }

    void BaseAction::LogUndoComplete() const
    {
        TRY
            std::wstring message = GetUndoMessageComplete();
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

    void BaseAction::Redo() 
    {
        TRY
            LogRedoStart();
            OnRedo();
            LogRedoComplete();
        CATCH
    }

    void BaseAction::Undo()
    {
        TRY
            LogUndoStart();
            OnUndo();
            LogUndoComplete();
        CATCH
    }
}
