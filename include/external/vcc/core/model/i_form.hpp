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
            virtual void onInitialize() = 0;
            virtual void onInitializeComponents() = 0;
            virtual void onInitializeValues() = 0;

            // Close
            virtual bool OnIsClosable() const = 0;
            virtual bool OnClose() = 0;
        public:
            // Log
            virtual std::shared_ptr<LogConfig> getLogConfig() const = 0;
            virtual void setLogConfig(std::shared_ptr<LogConfig> logConfig) = 0;
            
            // Action
            virtual std::shared_ptr<ActionManager> getActionManager() const = 0;
            virtual void setActionManager(std::shared_ptr<ActionManager> actionManager) = 0;

            // Thead
            virtual std::shared_ptr<ThreadManager> getThreadManager() const = 0;
            virtual void setThreadManager(std::shared_ptr<ThreadManager> threadManager) = 0;

            // State
            virtual State getState() const = 0;
            virtual bool isClosable() const = 0;
            virtual bool isClosed() const = 0;

            // Initialize
            virtual void initialize() = 0;
            virtual void initializeComponents() = 0;
            virtual void initializeValue() = 0;

            // Action
            virtual std::shared_ptr<IResult> doAction(const int64_t &formProperty, std::shared_ptr<IObject> argument) = 0;
            virtual std::shared_ptr<IResult> executeAction(std::shared_ptr<IAction> action, bool isNoHistory) = 0;
            virtual int64_t getActionCurrentSeqNo() const = 0;
            virtual int64_t getActionFirstSeqNo() const = 0;
            virtual int64_t getActionLastSeqNo() const = 0;
            
            virtual std::shared_ptr<IResult> redoAction(const int64_t &noOfStep = 1) = 0;
            virtual std::shared_ptr<IResult> redoActionToSeqNo(const int64_t &seqNo) = 0;

            virtual std::shared_ptr<IResult> undoAction(const int64_t &noOfStep = 1) = 0;
            virtual std::shared_ptr<IResult> undoActionToSeqNo(const int64_t &seqNo) = 0;

            virtual int64_t clearAction() = 0;
            virtual int64_t truncateAction() = 0;

            // Close
            virtual bool Close(bool isForce = false) = 0;
    };
}
