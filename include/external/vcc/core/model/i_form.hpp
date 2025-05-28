#pragma once

#include <memory>

#include "action_manager.hpp"
#include "i_object.hpp"
#include "i_result.hpp"
#include "log_config.hpp"
#include "state.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    class IForm
    {
        protected:
            IForm() = default;
            virtual ~IForm() = default;
        
            // Initialize
            virtual void OnInitialize() = 0;
            virtual void OnInitializeComponents() = 0;
            virtual void OnInitializeValues() = 0;

            // Close
            virtual bool OnIsClosable() const = 0;
            virtual bool OnClose() = 0;
        public:
            // Log
            virtual std::shared_ptr<LogConfig> GetLogConfig() const = 0;
            virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) = 0;
            
            // Action
            virtual std::shared_ptr<ActionManager> GetActionManager() const = 0;
            virtual void SetActionManager(std::shared_ptr<ActionManager> actionManager) = 0;

            // Thead
            virtual std::shared_ptr<ThreadManager> GetThreadManager() const = 0;
            virtual void SetThreadManager(std::shared_ptr<ThreadManager> threadManager) = 0;

            // State
            virtual State GetState() const = 0;
            virtual bool IsClosable() const = 0;
            virtual bool IsClosed() const = 0;

            // Initialize
            virtual void Initialize() = 0;
            virtual void InitializeComponents() = 0;
            virtual void InitializeValue() = 0;

            // Action
            virtual std::shared_ptr<IResult> DoAction(const int64_t &formProperty, std::shared_ptr<IObject> argument) = 0;
            virtual std::shared_ptr<IResult> ExecuteAction(std::shared_ptr<IAction> action, bool isNoHistory) = 0;
            virtual int64_t GetActionCurrentSeqNo() const = 0;
            virtual int64_t GetActionFirstSeqNo() const = 0;
            virtual int64_t GetActionLastSeqNo() const = 0;
            
            virtual std::shared_ptr<IResult> RedoAction(const int64_t &noOfStep = 1) = 0;
            virtual std::shared_ptr<IResult> RedoActionToSeqNo(const int64_t &seqNo) = 0;

            virtual std::shared_ptr<IResult> UndoAction(const int64_t &noOfStep = 1) = 0;
            virtual std::shared_ptr<IResult> UndoActionToSeqNo(const int64_t &seqNo) = 0;

            virtual int64_t ClearAction() = 0;
            virtual int64_t TruncateAction() = 0;

            // Close
            virtual bool Close(bool isForce = false) = 0;
    };
}
