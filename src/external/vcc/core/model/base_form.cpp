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

    void BaseForm::SetLogConfig(std::shared_ptr<LogConfig> logConfig)
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

    void BaseForm::SetActionManager(std::shared_ptr<ActionManager> actionManager)
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

    void BaseForm::SetThreadManager(std::shared_ptr<ThreadManager> threadManager)
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

    void BaseForm::Initialize()
    {
        TRY
            InitializeComponents();
            InitializeValue();
            OnInitialize();
        CATCH
    }

    void BaseForm::InitializeComponents()
    {
        TRY
            OnInitialize();
        CATCH
    }

    void BaseForm::InitializeValue()
    {
        TRY
            OnInitializeValues();
        CATCH
    }

    std::shared_ptr<IResult> BaseForm::ExecuteAction(std::shared_ptr<IAction> action, bool isNoHistory)
    {
        TRY
            auto actionManager = GetActionManager();
            if (isNoHistory || actionManager == nullptr)
                return action->Redo();
            else
                return actionManager->DoAction(action);
        CATCH
        return nullptr;
    }
    
    int64_t BaseForm::GetActionCurrentSeqNo() const
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetCurrentSeqNo();
        CATCH
        return -1;
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
    
    std::shared_ptr<IResult> BaseForm::RedoAction(const int64_t &noOfStep)
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->Redo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::RedoActionToSeqNo(const int64_t &seqNo)
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->RedoToSeqNo(seqNo);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::UndoAction(const int64_t &noOfStep)
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->Undo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::UndoActionToSeqNo(const int64_t &seqNo)
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->UndoToSeqNo(seqNo);
        CATCH
        return nullptr;
    }

    int64_t BaseForm::ClearAction()
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetFirstSeqNo();
        CATCH
        return -1;
    }

    int64_t BaseForm::TruncateAction()
    {
        TRY
            auto actionManager = GetActionManager();
            if (actionManager != nullptr)
                return actionManager->GetFirstSeqNo();
        CATCH
        return -1;
    }

    bool BaseForm::Close(bool isForce)
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