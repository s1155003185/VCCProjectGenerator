#pragma once

#include "base_factory.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"

namespace vcc
{
    class BasePropertyAccessorFactory : public BaseFactory
    {
        protected:
            BasePropertyAccessorFactory() = default;
            virtual ~BasePropertyAccessorFactory() {}
    };
}
