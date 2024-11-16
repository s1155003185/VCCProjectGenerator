#pragma once
#include "i_object.hpp"
#include "class_macro.hpp"

#include "object_type.hpp"

namespace vcc
{
    class BaseObject : public IObject
    {
        protected:
            mutable ObjectType _ObjectType = ObjectType::NA;

            BaseObject() {}
            BaseObject(ObjectType type) : _ObjectType(type) { }
            virtual ~BaseObject() {}

        public:
            const ObjectType& GetObjectType() const override
            { 
                return _ObjectType;
            } 
            
            void SetObjectType(ObjectType value) const
            {
                _ObjectType = value;
            }
    };
}
