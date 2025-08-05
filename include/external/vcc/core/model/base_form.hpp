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
        BaseForm(const ObjectType &type) : BaseObject(type) { initialize(); }
        virtual ~BaseForm() = default;

        virtual void OnInitialize() override {}
        virtual void OnInitializeComponents() override {}
        virtual void OnInitializeValues() override {}
        virtual bool OnIsClosable() const override { return true; }
        virtual bool OnClose() override { return true; }
    public:
        // Log
        virtual std::shared_ptr<LogConfig> getLogConfig() const override;
        virtual void setLogConfig(std::shared_ptr<LogConfig> logConfig) override;
        
        // Action
        virtual std::shared_ptr<ActionManager> getActionManager() const override;
        virtual void setActionManager(std::shared_ptr<ActionManager> actionManager) override;

        // Thead
        virtual std::shared_ptr<ThreadManager> getThreadManager() const override;
        virtual void setThreadManager(std::shared_ptr<ThreadManager> threadManager) override;

        // State
        virtual State getState() const override;
        virtual bool IsClosable() const override;
        virtual bool IsClosed() const override;

        // Initialize
        virtual void initialize() override;
        virtual void initializeComponents() override;
        virtual void initializeValue() override;
        
        // Action
        virtual std::shared_ptr<IResult> executeAction(std::shared_ptr<IAction> action, bool isNoHistory) override;
        virtual int64_t getActionCurrentSeqNo() const override;
        virtual int64_t getActionFirstSeqNo() const override;
        virtual int64_t getActionLastSeqNo() const override;
        
        virtual std::shared_ptr<IResult> RedoAction(const int64_t &noOfStep = 1) override;
        virtual std::shared_ptr<IResult> RedoActionToSeqNo(const int64_t &seqNo) override;

        virtual std::shared_ptr<IResult> UndoAction(const int64_t &noOfStep = 1) override;
        virtual std::shared_ptr<IResult> UndoActionToSeqNo(const int64_t &seqNo) override;

        virtual int64_t clearAction() override;
        virtual int64_t TruncateAction() override;

        // Close
        virtual bool Close(bool isForce = false) override;
    };
}
