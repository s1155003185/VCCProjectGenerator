#pragma once

#include <memory>
#include <string>

#include "base_object.hpp"
#include "i_action.hpp"
#include "log_config.hpp"
#include "thread_manager.hpp"

namespace vcc
{
    class BaseAction : public IAction, public BaseObject
    {
        private:
            mutable size_t _SeqNo = 0;
            mutable std::shared_ptr<LogConfig> _LogConfig = nullptr;
            mutable std::shared_ptr<ThreadManager> _ThreadManager = nullptr;

        protected:
            BaseAction() : BaseObject() {}
            virtual ~BaseAction() {}

            virtual void LogRedo() const;
            virtual void LogUndo() const;
        public:
            // No Clone Method for Action
            virtual std::shared_ptr<IObject> Clone() const override;
            
            // Log
            virtual std::shared_ptr<LogConfig> GetLogConfig() const override;
            virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const override;

            // Thead
            virtual std::shared_ptr<ThreadManager> GetThreadManager() const override;
            virtual void SetThreadManager(std::shared_ptr<ThreadManager> threadManager) const override;

            virtual size_t GetSeqNo() const override;
            virtual void SetSeqNo(const size_t &seqNo) const override;

            virtual void Redo() const override;
            virtual void Undo() const override;
    };
}
