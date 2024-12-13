#pragma once

#include "i_form.hpp"
#include "action_manager.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "state.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    class BaseForm : public IForm, public BaseObject
    {        
    protected:
        mutable std::shared_ptr<LogConfig> _LogConfig = nullptr;
        mutable std::shared_ptr<ActionManager> _ActionManager = nullptr;
        mutable std::shared_ptr<ThreadManager> _ThreadManager = nullptr;

        mutable State _State = State::Active;
        
        BaseForm() : BaseForm(ObjectType::NA) {}
        BaseForm(const ObjectType &type) : BaseObject(type) { Initialize(); }
        virtual ~BaseForm() = default;

        virtual void OnInitialize() const override {}
        virtual void OnInitializeComponents() const override {}
        virtual void OnInitializeValues() const override {}
        virtual bool OnIsClosable() const override { return true; }
        virtual bool OnClose() const override { return true; }
    public:
        // Log
        virtual std::shared_ptr<LogConfig> GetLogConfig() const override;
        virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const override;
        
        // Action
        virtual std::shared_ptr<ActionManager> GetActionManager() const override;
        virtual void SetActionManager(std::shared_ptr<ActionManager> actionManager) const override;

        // Thead
        virtual std::shared_ptr<ThreadManager> GetThreadManager() const override;
        virtual void SetThreadManager(std::shared_ptr<ThreadManager> threadManager) const override;

        // State
        virtual State GetState() const override;
        virtual bool IsClosable() const override;
        virtual bool IsClosed() const override;

        // Initialize
        virtual void Initialize() const override;
        virtual void InitializeComponents() const override;
        virtual void InitializeValue() const override;
        
        // Action
        virtual int64_t GetActionFirstSeqNo() const override;
        virtual int64_t GetActionLastSeqNo() const override;
        
        virtual int64_t RedoAction(const int64_t &noOfStep = 1) const override;
        virtual int64_t RedoActionToSeqNo(const int64_t &seqNo) const override;

        virtual int64_t UndoAction(const int64_t &noOfStep = 1) const override;
        virtual int64_t UndoActionToSeqNo(const int64_t &seqNo) const  override;

        virtual int64_t ClearAction() const override;
        virtual int64_t TruncateAction() const override;

        // Close
        virtual bool Close(bool isForce = false) const override;
    };
}
