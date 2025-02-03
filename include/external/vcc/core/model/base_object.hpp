#pragma once

#include <memory>

#include "i_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"

namespace vcc
{
    // Should not change this value outside base_object.hpp
    static size_t __NextObjectId = 0;
    
    class BaseObject : public IObject
    {
        private:
            mutable size_t _ObjectId = __NextObjectId++;
        protected:
            mutable ObjectType _ObjectType = ObjectType::NA;
            mutable std::shared_ptr<IObject> _ParentObject = nullptr;

            BaseObject() = default;
            BaseObject(const ObjectType &type) : _ObjectType(type) {}
            virtual ~BaseObject() {}

        public:
            virtual size_t GetObjectId() const override
            {
                return _ObjectId;
            }

            virtual const ObjectType& GetObjectType() const override
            { 
                return _ObjectType;
            }

            virtual std::shared_ptr<IObject> GetParentObject() const override
            {
                return _ParentObject;
            }

            virtual void SetParentObject(std::shared_ptr<IObject> parentObject) const override
            {
                _ParentObject = parentObject;
            }
            
    };
}
