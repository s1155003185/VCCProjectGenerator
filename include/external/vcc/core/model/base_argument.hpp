#pragma once

#include "i_argument.hpp"
#include "base_object.hpp"
#include "object_type.hpp"

namespace vcc
{
    class BaseArgument : public IArgument, public BaseObject
    {        
    protected:
        BaseArgument() : BaseArgument(ObjectType::NA) {}
        BaseArgument(const ObjectType &objectType) : BaseObject(objectType) {}
        virtual ~BaseArgument() = default;
    };
}
