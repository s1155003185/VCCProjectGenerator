#pragma once

#include <assert.h>
#include <memory>
#include <mutex>
#include <shared_mutex>

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
            //mutable std::shared_mutex _mutex;
            size_t _SeqNo = 0;
        protected:
            BaseAction() : BaseObject() {}
            virtual ~BaseAction() {}

            virtual void _LogRedo() 
            { 
                std::wstring message = this->_GetRedoMessage();
                if (!IsBlank(message)) {
                    std::shared_ptr<LogConfig> defaultProperty = std::make_shared<LogConfig>();
                    LogService::LogInfo(defaultProperty.get(), L"", message); 
                }
            }
            
            virtual void _LogUndo() 
            { 
                std::wstring message = this->_GetUndoMessage();
                if (!IsBlank(message)) {
                    std::shared_ptr<LogConfig> defaultProperty = std::make_shared<LogConfig>();
                    LogService::LogInfo(defaultProperty.get(), L"", message); 
                }
            }

        public:
            // No Clone Method for Action
            virtual std::shared_ptr<IObject> Clone() const override { assert(false); return nullptr; }
            
            virtual size_t GetSeqNo() override 
            { 
                //std::shared_lock lock(this->_mutex); 
                return this->_SeqNo;
            }
            virtual void SetSeqNo(const size_t &seqNo) override 
            { 
                //std::unique_lock lock(this->_mutex); 
                this->_SeqNo = seqNo; 
            }

            virtual void Redo() override 
            {
                //std::unique_lock lock(this->_mutex);

                this->_DoRedo();
                this->_LogRedo();
            }

            virtual void Undo() override
            {
                //std::unique_lock lock(this->_mutex);

                this->_DoUndo();
                this->_LogUndo();
            }
    };
}
