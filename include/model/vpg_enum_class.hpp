#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "vector_helper.hpp"

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
    , ActionArgument
    , Result
};

enum class VPGEnumClassPropertyType
{
    NA
    , Property
    , Manager
    , Action
};

enum class VPGEnumClassMacroType
{
    NA
    , Getcustom
    , Setcustom
    , GetcustomSptr
    , SetcustomSptr
    , Getset
    , GetsetValidate
    , GetsetSptr
    , GetsetSptrNull
    , GetsetValidateSptrNull
    , Vector
    , VectorValidate
    , VectorSptr
    , VectorValidateSptr
    , Set
    , SetValidate
    , SetSptr
    , SetValidateSptr
    , Map
    , MapValidate
    , MapSptrR
    , MapValidateSptrR
    , OrderedMap
    , OrderedMapValidate
    , OrderedMapSptrR
    , OrderedMapValidateSptrR
    , ManagerSptr
    , ManagerSptrNull
    , ManagerSptrParent
    , Action
    , ActionWithArgSptr
};

class VPGEnumClassProperty : public BaseObject
{
    friend class VPGEnumClassReader;

    private:
        mutable std::wstring _DefaultValue = L"";
        mutable VPGEnumClassPropertyAccessMode _AccessMode = VPGEnumClassPropertyAccessMode::ReadWrite;

    GETSET(VPGEnumClassPropertyType, PropertyType, VPGEnumClassPropertyType::NA);
    GETSET(std::wstring, Enum, L"");
    GETSET(int64_t, EnumValue, 0);
    GETSET(std::wstring, Macro, L"");
    GETSET(std::wstring, Type1, L"");
    GETSET(std::wstring, Type2, L"");
    GETSET(std::wstring, PropertyName, L"");
    GETSET(std::wstring, Validate, L"")
    GETCUSTOM(std::wstring, DefaultValue, if (_InitializeProperties.empty()) return _DefaultValue; return Concat(_InitializeProperties, L", ");)
    SETCUSTOM(DefaultValue, std::wstring, _DefaultValue = value;)
    GETSET(std::wstring, Command, L"");

    // Properties
    GETCUSTOM(bool, IsGeneralType, return !GetIsCollection();)
    GETCUSTOM(bool, IsCustom, return _Macro.find(L"(") != std::wstring::npos ? IsContain(_Macro.substr(0, _Macro.find(L"(")), L"CUSTOM") : false;)
    GETCUSTOM(bool, IsObject, return _Macro.find(L"(") != std::wstring::npos ? IsContain(_Macro.substr(0, _Macro.find(L"(")), L"_SPTR") : false;)
    GETCUSTOM(bool, IsVector, return IsStartWith(_Macro, L"VECTOR");)
    GETCUSTOM(bool, IsMap, return IsStartWith(_Macro, L"MAP");)
    GETCUSTOM(bool, IsOrderedMap, return IsStartWith(_Macro, L"ORDERED_MAP");)
    GETCUSTOM(bool, IsSet, return IsContain(std::vector<VPGEnumClassMacroType>{ VPGEnumClassMacroType::Set, VPGEnumClassMacroType::SetSptr, VPGEnumClassMacroType::SetValidate, VPGEnumClassMacroType::SetValidateSptr }, _MacroType);)
    GETCUSTOM(bool, IsAction, return IsStartWith(_Macro, L"ACTION");)
    GETCUSTOM(bool, IsManager, return IsStartWith(_Macro, L"MANAGER");)
    GETCUSTOM(bool, IsCollection, return GetIsVector() || GetIsMap() || GetIsOrderedMap() || GetIsSet();)
    GETCUSTOM(bool, IsHavingValidate, return _Macro.find(L"(") != std::wstring::npos ? IsContain(_Macro.substr(0, _Macro.find(L"(")), L"VALIDATE") : false;)

    // Macro
    GETSET(VPGEnumClassMacroType, MacroType, VPGEnumClassMacroType::NA);
    GETCUSTOM(VPGEnumClassPropertyAccessMode, AccessMode, 
        switch (_MacroType) {
        case VPGEnumClassMacroType::Getcustom:
        case VPGEnumClassMacroType::GetcustomSptr:
        {
            switch (_AccessMode)
            {
            case VPGEnumClassPropertyAccessMode::ReadWrite:
                return VPGEnumClassPropertyAccessMode::ReadOnly;
            case VPGEnumClassPropertyAccessMode::WriteOnly:
                return VPGEnumClassPropertyAccessMode::NoAccess;
            default:
                break;
            }
            break;
        }
        case VPGEnumClassMacroType::Setcustom:
        case VPGEnumClassMacroType::SetcustomSptr:
        {
            switch (_AccessMode)
            {
            case VPGEnumClassPropertyAccessMode::ReadWrite:
                return VPGEnumClassPropertyAccessMode::WriteOnly;
            case VPGEnumClassPropertyAccessMode::ReadOnly:
                return VPGEnumClassPropertyAccessMode::NoAccess;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
        return _AccessMode;
    )
    SETCUSTOM(AccessMode, VPGEnumClassPropertyAccessMode, _AccessMode = value;)

    GETSET(bool, IsInherit, false);
    
    // Properties
    VECTOR(std::wstring, InitializeProperties)

    // Action
    GETSET(bool, IsNoHistory, false);
    GETSET(std::wstring, ActionResultRedoClass, L"OperationResult")
    GETSET(std::wstring, ActionResultUndoClass, L"OperationResult")

    // Json
    GETSET(bool, IsNoJson, false)

    public:
        VPGEnumClassProperty() : BaseObject() {}
        virtual ~VPGEnumClassProperty() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGEnumClassProperty>(*this);
        }

        bool GetIsInitializeInClassConstructorNeeded() const
        {
            return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::GetsetSptr, VPGEnumClassMacroType::ManagerSptr}, _MacroType)
                || (!IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Getcustom, VPGEnumClassMacroType::GetcustomSptr}, _MacroType) && !_InitializeProperties.empty());
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

    // Files
    VECTOR(std::wstring, IncludeSystemFiles)
    VECTOR(std::wstring, IncludeCustomFiles)

    // Properties
    MAP(std::wstring, std::wstring, PrivateProperties)
    MAP(std::wstring, std::wstring, ProtectedProperties)

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