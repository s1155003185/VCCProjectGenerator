#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"

using namespace vcc;

enum class VPGEnumClassPropertyAccessMode
{
    ReadWrite,
    ReadOnly,
    WriteOnly,
    NoAccess
};

enum class VPGEnumClassType
{
    Object
    , Form
};

class VPGEnumClassProperty : public BaseObject
{
    friend class VPGEnumClassReader;
    GETSET(std::wstring, Enum, L"");
    GETSET(int64_t, EnumValue, 0);
    GETSET(std::wstring, Macro, L"");
    GETSET(std::wstring, Type1, L"");
    GETSET(std::wstring, Type2, L"");
    GETSET(std::wstring, PropertyName, L"");
    GETSET(std::wstring, DefaultValue, L"");
    GETSET(std::wstring, Command, L"");

    GETSET(VPGEnumClassPropertyAccessMode, AccessMode, VPGEnumClassPropertyAccessMode::ReadWrite);
    GETSET(bool, IsInherit, false);

    public:
        VPGEnumClassProperty() : BaseObject() {}
        virtual ~VPGEnumClassProperty() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGEnumClassProperty>(*this);
        }
};

class VPGEnumClass : public BaseObject
{
    friend class VPGEnumClassReader;
    GETSET(std::wstring, Namespace, L"")
    GETSET(VPGEnumClassType, Type, VPGEnumClassType::Object)
    GETSET(std::wstring, Name, L"")
    GETSET(std::wstring, Command, L"")
    VECTOR_SPTR(VPGEnumClassProperty, Properties)

    // Log
    GETSET(bool, IsLogConfigIndependent, false)

    // Action
    GETSET(bool, IsActionManagerIndependent, false)

    // Thread
    GETSET(bool, IsThreadManagerIndependent, false)

    // Json
    GETSET(bool, IsJson, false)
    MAP(std::wstring, std::wstring, JsonAttributes)
    GETSET(std::wstring, InheritClass, L"")
    MAP(std::wstring, std::wstring, InheritClassAttributes)


    public:
        VPGEnumClass() : BaseObject() {}
        virtual ~VPGEnumClass() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            std::shared_ptr<VPGEnumClass> obj = std::make_shared<VPGEnumClass>(*this);
            obj->CloneProperties(this->GetProperties());
            return obj;
        }
};