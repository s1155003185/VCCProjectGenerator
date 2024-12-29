#pragma once

#include <memory>
#include <string>

#include "base_object.hpp"
#include "i_action.hpp"
#include "log_config.hpp"

namespace vcc
{
    class BaseAction : public IAction, public BaseObject
    {
        protected:
            mutable size_t _SeqNo = 0;
            mutable std::shared_ptr<LogConfig> _LogConfig = nullptr;

            BaseAction() : BaseObject() {}
            virtual ~BaseAction() {}
        
            virtual void LogRedoStart() const;
            virtual void LogRedoComplete() const;
            virtual void LogUndoStart() const;
            virtual void LogUndoComplete() const;
        public:
            // No Clone Method for Action
            virtual std::shared_ptr<IObject> Clone() const override;
            
            // Log
            virtual std::shared_ptr<LogConfig> GetLogConfig() const override;
            virtual void SetLogConfig(std::shared_ptr<LogConfig> logConfig) const override;

            virtual size_t GetSeqNo() const override;
            virtual void SetSeqNo(const size_t &seqNo) const override;

            virtual void Redo() override;
            virtual void Undo() override;
    };
}
