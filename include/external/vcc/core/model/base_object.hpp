#pragma once

#include <memory>

#include "i_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"

namespace vcc
{
    class BaseObject : public IObject
    {
        protected:
            mutable ObjectType _ObjectType = ObjectType::NA;
            mutable std::shared_ptr<IObject> _ParentObject = nullptr;

            BaseObject() = default;
            BaseObject(const ObjectType &type) { _ObjectType = type; }
            virtual ~BaseObject() {}

        public:
            const ObjectType& GetObjectType() const override
            { 
                return _ObjectType;
            }

            std::shared_ptr<IObject> GetParentObject() const override
            {
                return _ParentObject;
            }

            void SetParentObject(std::shared_ptr<IObject> parentObject) const override
            {
                _ParentObject = parentObject;
            }
            
    };
}
