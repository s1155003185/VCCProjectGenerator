#pragma once
#include "i_action.hpp"
#include "class_macro.hpp"

#include "action_type.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "object_type.hpp"
#include "string_helper.hpp"

#include <memory>
#include <mutex>
#include <shared_mutex>

namespace vcc
{
    template <typename Derived>
    class BaseAction : public IAction
    {
        private:
            ObjectType _ObjectType = ObjectType::NA;
        GETSET(ActionType, Type, ActionType::NA)

        private:
            //mutable std::shared_mutex _mutex;

            size_t _SeqNo = 0;
            BaseAction() {}
        protected:
            BaseAction(ActionType type) : IAction() { this->_Type = type; }
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
            virtual size_t GetSeqNo() override 
            { 
                //std::shared_lock lock(this->_mutex); 
                return this->_SeqNo;
            }
            virtual void SetSeqNo(size_t seqNo) override 
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

            virtual const ObjectType& GetObjectType() const override
            {
                return _ObjectType;
            }
            
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<Derived>(static_cast<const Derived&>(*this));
            }
    };
}
