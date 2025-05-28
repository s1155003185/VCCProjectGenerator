#pragma once

#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_form.hpp"
#include "i_result.hpp"
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

        virtual void OnInitialize() override {}
        virtual void OnInitializeComponents() override {}
        virtual void OnInitializeValues() override {}
        virtual bool OnIsClosable() const override { return true; }
        virtual bool OnClose() override { return true; }
    public:
        // Log
        virtual std::shared_ptr<LogConfig> GetLogConfig() const override;
        virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) override;
        
        // Action
        virtual std::shared_ptr<ActionManager> GetActionManager() const override;
        virtual void SetActionManager(std::shared_ptr<ActionManager> actionManager) override;

        // Thead
        virtual std::shared_ptr<ThreadManager> GetThreadManager() const override;
        virtual void SetThreadManager(std::shared_ptr<ThreadManager> threadManager) override;

        // State
        virtual State GetState() const override;
        virtual bool IsClosable() const override;
        virtual bool IsClosed() const override;

        // Initialize
        virtual void Initialize() override;
        virtual void InitializeComponents() override;
        virtual void InitializeValue() override;
        
        // Action
        virtual std::shared_ptr<IResult> ExecuteAction(std::shared_ptr<IAction> action, bool isNoHistory) override;
        virtual int64_t GetActionCurrentSeqNo() const override;
        virtual int64_t GetActionFirstSeqNo() const override;
        virtual int64_t GetActionLastSeqNo() const override;
        
        virtual std::shared_ptr<IResult> RedoAction(const int64_t &noOfStep = 1) override;
        virtual std::shared_ptr<IResult> RedoActionToSeqNo(const int64_t &seqNo) override;

        virtual std::shared_ptr<IResult> UndoAction(const int64_t &noOfStep = 1) override;
        virtual std::shared_ptr<IResult> UndoActionToSeqNo(const int64_t &seqNo) override;

        virtual int64_t ClearAction() override;
        virtual int64_t TruncateAction() override;

        // Close
        virtual bool Close(bool isForce = false) override;
    };
}
