#pragma once
#include "i_object.hpp"
#include "class_macro.hpp"

#include "object_type.hpp"

namespace vcc
{
    class BaseObject : public IObject
    {
        THREAD_SAFE
        GET(ObjectType, Type, ObjectType::NA)

        protected:
            BaseObject() {}
            BaseObject(ObjectType type) : BaseObject() { this->_Type = type; }
            ~BaseObject() {}
    };
}
