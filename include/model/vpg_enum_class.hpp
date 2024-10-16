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

class VPGEnumClassProperty : public BaseObject<VPGEnumClassProperty>
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
};

class VPGEnumClass : public BaseObject<VPGEnumClass>
{
    friend class VPGEnumClassReader;
    GETSET(std::wstring, Namespace, L"");
    GETSET(std::wstring, Name, L"");
    GETSET(std::wstring, Command, L"");
    VECTOR_SPTR(VPGEnumClassProperty, Properties);

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