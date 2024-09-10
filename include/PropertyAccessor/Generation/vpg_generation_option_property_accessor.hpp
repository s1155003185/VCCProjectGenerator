#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGGenerationOptionExportPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGGenerationOptionExportPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGGenerationOptionExportPropertyAccessor() {}
};

class VPGGenerationOptionPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGGenerationOptionPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGGenerationOptionPropertyAccessor() {}
};
