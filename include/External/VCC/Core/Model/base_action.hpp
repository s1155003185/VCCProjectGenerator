#pragma once
#include "i_action.hpp"
#include "class_macro.hpp"

#include "action_type.hpp"
#include "log_service.hpp"

namespace vcc
{
    class BaseAction : public IAction
    {
        THREAD_SAFE
        GET(ActionType, Type, ActionType::NA)

        private:
            size_t _SeqNo = 0;
            BaseAction() {}
        protected:
            BaseAction(ActionType type) : IAction() { this->_Type = type; }
            ~BaseAction() {}

            virtual void _LogRedo() 
            { 
                std::wstring message = this->_GetRedoMessage();
                if (!message.empty()) 
                    LogService::LogInfo(message); 
            }
            
            virtual void _LogUndo() 
            { 
                std::wstring message = this->_GetUndoMessage();
                if (!message.empty()) 
                    LogService::LogInfo(message); 
            }

        public:   
            virtual size_t GetSeqNo() override { LOCK_GUAND; return this->_SeqNo; }
            virtual void SetSeqNo(size_t seqNo) override { LOCK_GUAND; this->_SeqNo = seqNo; }

            virtual void Redo() override 
            {
                LOCK_GUAND;
                this->_DoRedo();
                this->_LogRedo();
            }

            virtual void Undo() override
            {
                LOCK_GUAND;
                this->_DoUndo();
                this->_LogUndo();
            }
    };
}
