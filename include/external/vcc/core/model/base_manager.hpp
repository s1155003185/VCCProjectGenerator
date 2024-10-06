#pragma once

#include <memory>

#include "i_manager.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"

#include "manager_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseManager : public IManager, public BaseObject<Derived>
    {
        GETSET_SPTR_NULL(LogProperty, LogProperty);
        GETSET(ManagerType, Type, ManagerType::NA);
        
    protected:
        BaseManager() = default;
        BaseManager(std::shared_ptr<LogProperty> logProperty) { this->_LogProperty = logProperty; }
        BaseManager(std::shared_ptr<LogProperty> logProperty, ManagerType type) : BaseManager(logProperty) { this->_Type = type; }
        virtual ~BaseManager() {}
    };
}
