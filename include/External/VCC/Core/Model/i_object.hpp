#pragma once
#include <memory>
#include <string>

#include "object_type.hpp"

namespace vcc
{
    class IObject
    {
        public:
            IObject() {}
            virtual ~IObject() {}

            virtual const ObjectType& GetObjectType() const = 0;
            virtual std::shared_ptr<IObject> Clone() const = 0;
    };
}
