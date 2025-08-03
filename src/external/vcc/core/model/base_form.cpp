#include "base_form.hpp"

#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "log_config.hpp"

namespace vcc
{
    std::shared_ptr<LogConfig> BaseForm::getLogConfig() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _LogConfig == nullptr) ? baseForm->getLogConfig() : _LogConfig;
        CATCH
        return nullptr;
    }

    void BaseForm::setLogConfig(std::shared_ptr<LogConfig> logConfig)
    {
        _LogConfig = logConfig;
    }

    std::shared_ptr<ActionManager> BaseForm::getActionManager() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _ActionManager == nullptr) ? baseForm->getActionManager() : _ActionManager;
        CATCH
        return nullptr;
    }

    void BaseForm::setActionManager(std::shared_ptr<ActionManager> actionManager)
    {
        _ActionManager = actionManager;
    }

    std::shared_ptr<ThreadManager> BaseForm::getThreadManager() const
    {
        TRY
            auto baseForm = std::dynamic_pointer_cast<BaseForm>(_ParentObject);
            return (baseForm != nullptr && _ThreadManager == nullptr) ? baseForm->getThreadManager() : _ThreadManager;
        CATCH
        return nullptr;
    }

    void BaseForm::setThreadManager(std::shared_ptr<ThreadManager> threadManager)
    {
        _ThreadManager = threadManager;
    }

    State BaseForm::getState() const
    {
        TRY
            return _State;
        CATCH
        return State::Active;
    }

    bool BaseForm::isClosable() const
    {
        TRY
            return OnIsClosable();
        CATCH
        return false;
    }

    bool BaseForm::isClosed() const
    {
        TRY
            return _State == State::Closed;
        CATCH
        return false;
    }

    void BaseForm::initialize()
    {
        TRY
            initializeComponents();
            initializeValue();
            onInitialize();
        CATCH
    }

    void BaseForm::initializeComponents()
    {
        TRY
            onInitialize();
        CATCH
    }

    void BaseForm::initializeValue()
    {
        TRY
            onInitializeValues();
        CATCH
    }

    std::shared_ptr<IResult> BaseForm::executeAction(std::shared_ptr<IAction> action, bool isNoHistory)
    {
        TRY
            auto actionManager = getActionManager();
            if (isNoHistory || actionManager == nullptr)
                return action->Redo();
            else
                return actionManager->doAction(action);
        CATCH
        return nullptr;
    }

    int64_t BaseForm::getActionCurrentSeqNo() const
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->getCurrentSeqNo();
        CATCH
        return -1;
    }

    int64_t BaseForm::getActionFirstSeqNo() const
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->getFirstSeqNo();
        CATCH
        return -1;
    }
    
    int64_t BaseForm::getActionLastSeqNo() const
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->getLastSeqNo();
        CATCH
        return -1;
    }
    
    std::shared_ptr<IResult> BaseForm::redoAction(const int64_t &noOfStep)
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->Redo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::redoActionToSeqNo(const int64_t &seqNo)
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->redoToSeqNo(seqNo);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::undoAction(const int64_t &noOfStep)
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->Undo(noOfStep);
        CATCH
        return nullptr;
    }

    std::shared_ptr<IResult> BaseForm::undoActionToSeqNo(const int64_t &seqNo)
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->UndoToSeqNo(seqNo);
        CATCH
        return nullptr;
    }

    int64_t BaseForm::clearAction()
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->getFirstSeqNo();
        CATCH
        return -1;
    }

    int64_t BaseForm::truncateAction()
    {
        TRY
            auto actionManager = getActionManager();
            if (actionManager != nullptr)
                return actionManager->getFirstSeqNo();
        CATCH
        return -1;
    }

    bool BaseForm::Close(bool isForce)
    {
        TRY
            if (!isForce && !isClosable())
                return false;
            if (!OnClose())
                return false;
            _State = State::Closed;
            return true;
        CATCH
        return false;
    }
}