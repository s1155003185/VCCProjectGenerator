#pragma once
#include "i_object.hpp"
#include "class_macro.hpp"

#include "object_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseObject : public IObject
    {
        GET(ObjectType, Type, ObjectType::NA)

        protected:
            BaseObject() {}
            BaseObject(ObjectType type) : BaseObject() { this->_Type = type; }
            ~BaseObject() {}

        public:
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<Derived>(static_cast<const Derived&>(*this));
            }
    };
}
