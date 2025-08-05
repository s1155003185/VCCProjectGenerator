#pragma once
#include <memory>
#include <string>

#include "object_type.hpp"

namespace vcc
{
    class IObject : public std::enable_shared_from_this<IObject>
    {
        public:
            IObject() {}
            virtual ~IObject() {}

            virtual std::shared_ptr<IObject> sharedPtr() { return shared_from_this(); }
            virtual std::shared_ptr<const IObject> sharedPtr() const { return shared_from_this(); }

            virtual size_t getObjectId() const = 0;
            virtual const ObjectType& getObjectType() const = 0;
            virtual std::shared_ptr<IObject> getParentObject() const = 0;
            virtual void setParentObject(std::shared_ptr<IObject> parentObject) = 0;
            virtual std::shared_ptr<IObject> clone() const = 0;
    };
}
