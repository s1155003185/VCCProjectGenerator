#pragma once

#include <assert.h>
#include <memory>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_action.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    class BaseAction : public IAction, public BaseObject
    {
        private:
            mutable size_t _SeqNo = 0;
        protected:
            BaseAction() : BaseObject() {}
            virtual ~BaseAction() {}

            virtual void LogRedo() const
            { 
                std::wstring message = this->GetRedoMessage();
                if (!IsBlank(message)) {
                    std::shared_ptr<LogConfig> defaultProperty = std::make_shared<LogConfig>();
                    LogService::LogInfo(defaultProperty.get(), L"", message); 
                }
            }
            
            virtual void LogUndo() const
            { 
                std::wstring message = this->GetUndoMessage();
                if (!IsBlank(message)) {
                    std::shared_ptr<LogConfig> defaultProperty = std::make_shared<LogConfig>();
                    LogService::LogInfo(defaultProperty.get(), L"", message); 
                }
            }

        public:
            // No Clone Method for Action
            virtual std::shared_ptr<IObject> Clone() const override { assert(false); return nullptr; }
            
            virtual size_t GetSeqNo() const override
            { 
                return this->_SeqNo;
            }
            virtual void SetSeqNo(const size_t &seqNo) const override 
            { 
                this->_SeqNo = seqNo; 
            }

            virtual void Redo() const override 
            {
                this->OnRedo();
                this->LogRedo();
            }

            virtual void Undo() const override
            {
                this->OnUndo();
                this->LogUndo();
            }
    };
}
