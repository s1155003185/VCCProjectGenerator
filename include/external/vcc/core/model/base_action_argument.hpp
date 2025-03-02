#pragma once

#include "base_argument.hpp"
#include "object_type.hpp"

namespace vcc
{
    class BaseActionArgument : public BaseArgument
    {        
    protected:
        BaseActionArgument() : BaseActionArgument(ObjectType::NA) {}
        BaseActionArgument(const ObjectType &objectType) : BaseArgument(objectType) {}
        virtual ~BaseActionArgument() = default;
    };
}
