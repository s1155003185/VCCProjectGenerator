#pragma once

#include <memory>
#include <string>

#include "base_object.hpp"
#include "exception_macro.hpp"
#include "i_action.hpp"
#include "i_result.hpp"
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
            
            // Need to override if Acion has undo
            virtual std::shared_ptr<IResult> OnUndo() override { THROW_EXCEPTION_MSG(ExceptionType::NotImplement, L"OnUndo() has not been implemented"); return nullptr; };
            virtual std::wstring getUndoMessageStart() const override { THROW_EXCEPTION_MSG(ExceptionType::NotImplement, L"GetUndoMessageStart() has not been implemented"); return L""; }
            virtual std::wstring getUndoMessageComplete() const override { THROW_EXCEPTION_MSG(ExceptionType::NotImplement, L"GetUndoMessageComplete() has not been implemented"); return L""; }

        public:
            // No Clone Method for Action
            virtual std::shared_ptr<IObject> Clone() const override;
            
            // Log
            virtual std::shared_ptr<LogConfig> getLogConfig() const override;
            virtual void setLogConfig(std::shared_ptr<LogConfig> logConfig) const override;

            virtual size_t getSeqNo() const override;
            virtual void setSeqNo(const size_t &seqNo) const override;

            virtual std::shared_ptr<IResult> Redo() override;
            virtual std::shared_ptr<IResult> Undo() override;
    };
}
