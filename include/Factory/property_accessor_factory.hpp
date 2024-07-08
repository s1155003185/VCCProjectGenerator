#pragma once

#include <memory>

#include "base_factory.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"

using namespace vcc;

class PropertyAccessorFactory : public BaseFactory
{
    private:
        PropertyAccessorFactory() = default;
        virtual ~PropertyAccessorFactory() {}

    public:
        static std::shared_ptr<IPropertyAccessor> Create(std::shared_ptr<IObject> object);
};
