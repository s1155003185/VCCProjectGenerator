#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

class VPGMainFormAddWorkspaceFormArgumentPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    public:
        VPGMainFormAddWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGMainFormAddWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    public:
        VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    public:
        VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor() {}
};

class VPGMainFormPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGMainFormPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGMainFormPropertyAccessor() {}
};
