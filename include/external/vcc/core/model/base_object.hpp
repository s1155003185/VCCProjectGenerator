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
            virtual size_t getObjectId() const override
            {
                return _ObjectId;
            }

            virtual const ObjectType& getObjectType() const override
            { 
                return _ObjectType;
            }

            virtual std::shared_ptr<IObject> getParentObject() const override
            {
                return _ParentObject;
            }

            virtual void setParentObject(std::shared_ptr<IObject> parentObject) override
            {
                _ParentObject = parentObject;
            }
            
    };
}
