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