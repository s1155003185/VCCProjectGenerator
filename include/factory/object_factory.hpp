#pragma once

#include <memory>

#include "base_factory.hpp"
#include "i_object.hpp"
#include "object_type.hpp"

using namespace vcc;

class ObjectFactory : public BaseFactory
{
    private:
        ObjectFactory() = default;
        virtual ~ObjectFactory() {}

    public:
        static std::shared_ptr<IObject> Create(const ObjectType &objectType, std::shared_ptr<IObject> parentObject = nullptr);
};
