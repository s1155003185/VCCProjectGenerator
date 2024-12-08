#include "base_form.hpp"

#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "log_config.hpp"

namespace vcc
{
    std::shared_ptr<LogConfig> BaseForm::GetLogConfig() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _LogConfig == nullptr) ? baseForm->GetLogConfig() : _LogConfig;
        CATCH
        return nullptr;
    }

    void BaseForm::SetLogConfig(std::shared_ptr<LogConfig> logConfig) const
    {
        _LogConfig = logConfig;
    }
    
    std::shared_ptr<ActionManager> BaseForm::GetActionManager() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _ActionManager == nullptr) ? baseForm->GetActionManager() : _ActionManager;
        CATCH
        return nullptr;
    }

    void BaseForm::SetActionManager(std::shared_ptr<ActionManager> actionManager) const
    {
        _ActionManager = actionManager;
    }

    std::shared_ptr<ThreadManager> BaseForm::GetThreadManager() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _ThreadManager == nullptr) ? baseForm->GetThreadManager() : _ThreadManager;
        CATCH
        return nullptr;
    }

    void BaseForm::SetThreadManager(std::shared_ptr<ThreadManager> threadManager) const
    {
        _ThreadManager = threadManager;
    }

    State BaseForm::GetState() const
    {
        TRY
            return _State;
        CATCH
        return State::Active;
    }

    bool BaseForm::IsClosable() const
    {
        TRY
            return OnIsClosable();
        CATCH
        return false;
    }

    bool BaseForm::IsClosed() const
    {
        TRY
            return _State == State::Closed;
        CATCH
        return false;
    }

    void BaseForm::Initialize() const
    {
        TRY
            OnInitialize();
        CATCH
    }

    void BaseForm::Reload() const
    {
        TRY
            OnReload();
        CATCH
    }

    int64_t BaseForm::GetActionFirstSeqNo() const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetFirstSeqNo();
        CATCH
        return -1;
    }
    
    int64_t BaseForm::GetActionLastSeqNo() const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetLastSeqNo();
        CATCH
        return -1;
    }
    
    int64_t BaseForm::RedoAction(const int64_t &noOfStep) const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->Redo(noOfStep);
        CATCH
        return -1;
    }

    int64_t BaseForm::RedoActionToSeqNo(const int64_t &seqNo) const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->RedoToSeqNo(seqNo);
        CATCH
        return -1;
    }

    int64_t BaseForm::UndoAction(const int64_t &noOfStep) const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->Undo(noOfStep);
        CATCH
        return -1;
    }

    int64_t BaseForm::UndoActionToSeqNo(const int64_t &seqNo) const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->UndoToSeqNo(seqNo);
        CATCH
        return -1;
    }

    int64_t BaseForm::ClearAction() const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetFirstSeqNo();
        CATCH
        return -1;
    }

    int64_t BaseForm::TruncateAction() const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetFirstSeqNo();
        CATCH
        return -1;
    }

    bool BaseForm::Close(bool isForce) const
    {
        TRY
            if (!isForce && !IsClosable())
                return false;
            if (!OnClose())
                return false;
            _State = State::Closed;
            return true;
        CATCH
        return false;
    }
}