#pragma once
#include "i_action.hpp"
#include "class_macro.hpp"

#include "action_type.hpp"
#include "log_property.hpp"
#include "log_service.hpp"

#include <mutex>
#include <shared_mutex>

namespace vcc
{
    class BaseAction : public IAction
    {
        GET(ActionType, Type, ActionType::NA)

        private:
            mutable std::shared_mutex _mutex;

            size_t _SeqNo = 0;
            BaseAction() {}
        protected:
            BaseAction(ActionType type) : IAction() { this->_Type = type; }
            ~BaseAction() {}

            virtual void _LogRedo() 
            { 
                std::wstring message = this->_GetRedoMessage();
                if (!message.empty()) {
                    LogProperty defaultProperty;
                    LogService::LogInfo(defaultProperty, L"", message); 
                }
            }
            
            virtual void _LogUndo() 
            { 
                std::wstring message = this->_GetUndoMessage();
                if (!message.empty()) {
                    LogProperty defaultProperty;
                    LogService::LogInfo(defaultProperty, L"", message); 
                }
            }

        public:   
            virtual size_t GetSeqNo() override 
            { 
                std::shared_lock lock(this->_mutex); 
                return this->_SeqNo;
            }
            virtual void SetSeqNo(size_t seqNo) override 
            { 
                std::unique_lock lock(this->_mutex); 
                this->_SeqNo = seqNo; 
            }

            virtual void Redo() override 
            {
                std::unique_lock lock(this->_mutex);

                this->_DoRedo();
                this->_LogRedo();
            }

            virtual void Undo() override
            {
                std::unique_lock lock(this->_mutex);

                this->_DoUndo();
                this->_LogUndo();
            }
    };
}
