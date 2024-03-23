#pragma once
#include "i_manager.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"

#include "manager_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseManager : public IManager, public BaseObject<Derived>
    {
        GET(ManagerType, Type, ManagerType::NA)

    protected:
        BaseManager() {}
        BaseManager(ManagerType type) : BaseManager() { this->_Type = type; }
        virtual ~BaseManager() {}
    };
}
