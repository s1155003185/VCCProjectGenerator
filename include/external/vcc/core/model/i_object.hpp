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

            virtual std::shared_ptr<IObject> SharedPtr() { return shared_from_this(); }
            virtual std::shared_ptr<const IObject> SharedPtr() const { return shared_from_this(); }

            virtual size_t GetObjectId() const = 0;
            virtual const ObjectType& GetObjectType() const = 0;
            virtual std::shared_ptr<IObject> GetParentObject() const = 0;
            virtual void SetParentObject(std::shared_ptr<IObject> parentObject) = 0;
            virtual std::shared_ptr<IObject> Clone() const = 0;
    };
}
