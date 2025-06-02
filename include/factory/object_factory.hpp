#pragma once

#include <memory>

#include "base_factory.hpp"
#include "i_object.hpp"
#include "object_type.hpp"

class ObjectFactory : public vcc::BaseFactory
{
    private:
        ObjectFactory() = default;
        virtual ~ObjectFactory() {}

    public:
        static std::shared_ptr<vcc::IObject> Create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject = nullptr);
};
