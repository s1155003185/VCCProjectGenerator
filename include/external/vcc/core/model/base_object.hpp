#pragma once
#include "i_object.hpp"
#include "class_macro.hpp"

#include "object_type.hpp"

namespace vcc
{
    template <typename Derived>
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

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<Derived>(static_cast<const Derived&>(*this));
            }
    };
}
