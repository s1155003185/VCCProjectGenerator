#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "vector_helper.hpp"

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

class VPGEnumClassProperty : public vcc::BaseObject
{
    friend class VPGEnumClassReader;

    private:
        mutable std::wstring _Type1 = L"";
        mutable std::wstring _Type2 = L"";
        mutable std::wstring _DefaultValue = L"";
        mutable VPGEnumClassPropertyAccessMode _AccessMode = VPGEnumClassPropertyAccessMode::ReadWrite;

    GETSET(VPGEnumClassPropertyType, PropertyType, VPGEnumClassPropertyType::NA);
    GETSET(std::wstring, Enum, L"");
    GETSET(int64_t, EnumValue, 0);
    GETSET(std::wstring, Macro, L"");
    VECTOR(std::wstring, Type1Namespace);
    GETCUSTOM(std::wstring, Type1, return _Type1;);
    SETCUSTOM(Type1, std::wstring, _Type1Namespace = vcc::SplitString(value, {L"::"}); for (auto &str : _Type1Namespace) vcc::Trim(str);  _Type1 = _Type1Namespace.back(); _Type1Namespace.pop_back(); );
    GETCUSTOM(std::wstring, FullType1, std::vector<std::wstring> result = _Type1Namespace; result.push_back(GetType1());  return vcc::Concat(result, L"::");)
    VECTOR(std::wstring, Type2Namespace);
    GETCUSTOM(std::wstring, Type2, return _Type2;);
    SETCUSTOM(Type2, std::wstring, _Type2Namespace = vcc::SplitString(value, {L"::"}); for (auto &str : _Type2Namespace) vcc::Trim(str);  _Type2 = _Type2Namespace.back(); _Type2Namespace.pop_back(); );
    GETSET(std::wstring, PropertyName, L"");
    GETSET(std::wstring, Validate, L"")
    GETCUSTOM(std::wstring, DefaultValue, if (_InitializeProperties.empty()) return _DefaultValue; return vcc::Concat(_InitializeProperties, L", ");)
    SETCUSTOM(DefaultValue, std::wstring, _DefaultValue = value;)
    GETSET(std::wstring, Command, L"");

    // Properties
    GETCUSTOM(bool, IsGeneralType, return !GetIsCollection();)
    GETCUSTOM(bool, IsCustom, return _Macro.find(L"(") != std::wstring::npos ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"CUSTOM") : false;)
    GETCUSTOM(bool, IsObject, return _Macro.find(L"(") != std::wstring::npos ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"_SPTR") : false;)
    GETCUSTOM(bool, IsVector, return vcc::IsStartWith(_Macro, L"VECTOR");)
    GETCUSTOM(bool, IsMap, return vcc::IsStartWith(_Macro, L"MAP");)
    GETCUSTOM(bool, IsOrderedMap, return vcc::IsStartWith(_Macro, L"ORDERED_MAP");)
    GETCUSTOM(bool, IsSet, return vcc::IsContain(std::vector<VPGEnumClassMacroType>{ VPGEnumClassMacroType::Set, VPGEnumClassMacroType::SetSptr, VPGEnumClassMacroType::SetValidate, VPGEnumClassMacroType::SetValidateSptr }, _MacroType);)
    GETCUSTOM(bool, IsAction, return vcc::IsStartWith(_Macro, L"ACTION");)
    GETCUSTOM(bool, IsManager, return vcc::IsStartWith(_Macro, L"MANAGER");)
    GETCUSTOM(bool, IsCollection, return GetIsVector() || GetIsMap() || GetIsOrderedMap() || GetIsSet();)
    GETCUSTOM(bool, IsHavingValidate, return _Macro.find(L"(") != std::wstring::npos ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"VALIDATE") : false;)

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
            return vcc::IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::GetsetSptr, VPGEnumClassMacroType::ManagerSptr}, _MacroType)
                || (!vcc::IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Getcustom, VPGEnumClassMacroType::GetcustomSptr}, _MacroType) && !_InitializeProperties.empty());
        }
};

class VPGEnumClass : public vcc::BaseObject
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