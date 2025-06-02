#pragma once

#include <memory>

#include "base_factory.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"

class PropertyAccessorFactory : public vcc::BaseFactory
{
    private:
        PropertyAccessorFactory() = delete;
        virtual ~PropertyAccessorFactory() {}

    public:
        static std::shared_ptr<vcc::IPropertyAccessor> Create(std::shared_ptr<vcc::IObject> object);
};
