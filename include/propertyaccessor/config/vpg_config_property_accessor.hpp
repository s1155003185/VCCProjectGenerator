#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGConfigTemplatePropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigTemplatePropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigTemplatePropertyAccessor() {}
};

class VPGConfigBehaviorPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)

    public:
        VPGConfigBehaviorPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigBehaviorPropertyAccessor() {}
};

class VPGConfigInputPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigInputPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigInputPropertyAccessor() {}
};

class VPGConfigOutputPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigOutputPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigOutputPropertyAccessor() {}
};

class VPGConfigExportPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGConfigExportPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigExportPropertyAccessor() {}
};

class VPGConfigPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGConfigPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGConfigPropertyAccessor() {}
};
