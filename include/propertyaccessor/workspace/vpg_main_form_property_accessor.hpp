#pragma once

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGMainFormPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGMainFormPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGMainFormPropertyAccessor() {}
};
