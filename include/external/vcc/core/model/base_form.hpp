#pragma once

#include "i_form.hpp"
#include "action_manager.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "state.hpp"

namespace vcc
{
    class BaseForm : public IForm, public BaseObject
    {        
    protected:
        mutable std::shared_ptr<LogConfig> _LogConfig = nullptr;
        mutable std::shared_ptr<ActionManager> _ActionManager = nullptr;

        mutable State _State = State::Active;
        
        BaseForm() : BaseForm(ObjectType::NA) {}
        BaseForm(const ObjectType &type) : BaseObject(type) { Initialize(); }
        virtual ~BaseForm() = default;

        virtual void OnInitialize() const override {}
        virtual void OnReload() const override {}
        virtual bool OnIsClosable() const override { return true; }
        virtual bool OnClose() const override { return true; }
    public:
        // Log
        virtual std::shared_ptr<LogConfig> GetLogConfig() const override;
        virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const override;
        
        // Action
        virtual std::shared_ptr<ActionManager> GetActionManager() const override;
        virtual void SetActionManager(std::shared_ptr<ActionManager> actionManager) const override;

        // State
        virtual State GetState() const override;
        virtual bool IsClosable() const override;
        virtual bool IsClosed() const override;

        // Initialize
        virtual void Initialize() const override;
        virtual void Reload() const override;

        // Close
        virtual bool Close(bool isForce = false) const override;
    };
}
