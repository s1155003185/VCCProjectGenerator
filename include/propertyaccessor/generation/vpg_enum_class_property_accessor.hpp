#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

class VPGEnumClassAttributePropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGEnumClassAttributePropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGEnumClassAttributePropertyAccessor() {}
};

class VPGEnumClassPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGEnumClassPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGEnumClassPropertyAccessor() {}
};
