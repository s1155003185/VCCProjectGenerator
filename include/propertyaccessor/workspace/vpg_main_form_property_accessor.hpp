#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGMainFormAddWorkspaceFormArgumentPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGMainFormAddWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGMainFormAddWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    public:
        VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    public:
        VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGMainFormPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGMainFormPropertyAccessor() {}
};
