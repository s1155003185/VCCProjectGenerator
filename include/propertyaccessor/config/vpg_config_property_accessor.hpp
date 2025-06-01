#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

class VPGConfigTemplatePropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigTemplatePropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigTemplatePropertyAccessor() {}
};

class VPGConfigBehaviorPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)

    public:
        VPGConfigBehaviorPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigBehaviorPropertyAccessor() {}
};

class VPGConfigInputPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigInputPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigInputPropertyAccessor() {}
};

class VPGConfigOutputPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigOutputPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigOutputPropertyAccessor() {}
};

class VPGConfigExportPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigExportPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigExportPropertyAccessor() {}
};

class VPGConfigPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGConfigPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGConfigPropertyAccessor() {}
};
