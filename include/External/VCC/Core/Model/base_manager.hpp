#pragma once
#include "i_manager.hpp"
#include "class_macro.hpp"

#include "manager_type.hpp"

namespace vcc
{
    class BaseManager : public IManager
    {
        THREAD_SAFE
        GET(ManagerType, Type, ManagerType::NA)

    protected:
        BaseManager() {}
        BaseManager(ManagerType type) : BaseManager() { this->_Type = type; }
        ~BaseManager() {}
    };
}
