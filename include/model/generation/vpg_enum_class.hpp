// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"
#include "vpg_enum_class_type.hpp"

class VPGEnumClassAttribute : public vcc::BaseObject
{
    private:
        mutable VPGEnumClassAttributeAccessMode _AccessMode = VPGEnumClassAttributeAccessMode::ReadWrite;
        mutable std::wstring _DefaultValue = L"";


    GETSET(VPGEnumClassAttributeType, PropertyType, VPGEnumClassAttributeType::NA)
    GETSET(std::wstring, Enum, L"")
    GETSET(int64_t, EnumValue, 0)
    GETSET(std::wstring, Macro, L"")
    GETSET(VPGEnumClassMacroType, MacroType, VPGEnumClassMacroType::NA)
    GETSET(std::wstring, Type1, L"")
    GETCUSTOM(bool, IsType1Custom, return vcc::isCapital(vcc::splitString(getType1(), {L"::"}).back());)
    GETSET(std::wstring, Type2, L"")
    GETCUSTOM(bool, IsType2Custom, return vcc::isCapital(vcc::splitString(getType2(), {L"::"}).back());)
    GETSET(std::wstring, PropertyName, L"")
    GETSET(std::wstring, Validate, L"")
    GETCUSTOM(std::wstring, DefaultValue, if (_InitializeProperties.empty()) return _DefaultValue; return vcc::concat(_InitializeProperties, L", ");)
    SETCUSTOM(DefaultValue, std::wstring, _DefaultValue = value;)
    GETSET(std::wstring, Command, L"")
    GETCUSTOM(bool, IsGeneralType, return !getIsCollection();)
    GETCUSTOM(bool, IsCustom, return vcc::isContain(getMacro(), L"(") ? vcc::isContain(_Macro.substr(0, _Macro.find(L"(")), L"CUSTOM") : false;)
    GETCUSTOM(bool, IsObject, return vcc::isContain(getMacro(), L"(") ? vcc::isContain(_Macro.substr(0, _Macro.find(L"(")), L"_SPTR") : false;)
    GETCUSTOM(bool, IsVector, return vcc::isStartWith(_Macro, L"VECTOR");)
    GETCUSTOM(bool, IsMap, return vcc::isStartWith(_Macro, L"MAP");)
    GETCUSTOM(bool, IsOrderedMap, return vcc::isStartWith(_Macro, L"ORDERED_MAP");)
    GETCUSTOM(bool, IsSet, return vcc::isContain(std::vector<VPGEnumClassMacroType>{ VPGEnumClassMacroType::Set, VPGEnumClassMacroType::SetSptr, VPGEnumClassMacroType::SetValidate, VPGEnumClassMacroType::SetValidateSptr }, _MacroType);)
    GETCUSTOM(bool, IsAction, return vcc::isStartWith(_Macro, L"ACTION");)
    GETCUSTOM(bool, IsManager, return vcc::isStartWith(_Macro, L"MANAGER");)
    GETCUSTOM(bool, IsCollection, return getIsVector() || getIsMap() || getIsOrderedMap() || getIsSet();)
    GETCUSTOM(bool, IsHavingValidate, return vcc::isContain(getMacro(), L"(") ? vcc::isContain(_Macro.substr(0, _Macro.find(L"(")), L"VALIDATE") : false;)
    GETCUSTOM(VPGEnumClassAttributeAccessMode, AccessMode, 
                        switch (_MacroType) {
                        case VPGEnumClassMacroType::Getcustom:
                        case VPGEnumClassMacroType::GetcustomSptr:
                        {
                            switch (_AccessMode)
                            {
                            case VPGEnumClassAttributeAccessMode::ReadWrite:
                                return VPGEnumClassAttributeAccessMode::ReadOnly;
                            case VPGEnumClassAttributeAccessMode::WriteOnly:
                                return VPGEnumClassAttributeAccessMode::NoAccess;
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
                            case VPGEnumClassAttributeAccessMode::ReadWrite:
                                return VPGEnumClassAttributeAccessMode::WriteOnly;
                            case VPGEnumClassAttributeAccessMode::ReadOnly:
                                return VPGEnumClassAttributeAccessMode::NoAccess;
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
    SETCUSTOM(AccessMode, VPGEnumClassAttributeAccessMode, _AccessMode = value;)
    GETSET(bool, IsInherit, false)
    VECTOR(std::wstring, InitializeProperties)
    GETSET(bool, IsNoHistory, false)
    GETSET(std::wstring, ActionResultRedoClass, L"vcc::OperationResult")
    GETSET(std::wstring, ActionResultUndoClass, L"vcc::OperationResult")
    GETSET(bool, IsNoJson, false)
    GETCUSTOM(bool, IsInitializeInClassConstructorNeeded, 
                                                return vcc::isContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::GetsetSptr, VPGEnumClassMacroType::ManagerSptr}, _MacroType)
                                                    || (!vcc::isContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Getcustom, VPGEnumClassMacroType::GetcustomSptr}, _MacroType) && !_InitializeProperties.empty());)

    public:
        VPGEnumClassAttribute() : vcc::BaseObject(ObjectType::EnumClassAttribute) {}
        virtual ~VPGEnumClassAttribute() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGEnumClassAttribute>(*this);
        }
};

class VPGEnumClass : public vcc::BaseObject
{
    GETSET(VPGEnumClassType, Type, VPGEnumClassType::Object)
    GETSET(std::wstring, Name, L"")
    GETSET(std::wstring, Command, L"")
    VECTOR_SPTR(VPGEnumClassAttribute, Properties)
    VECTOR(std::wstring, IncludeSystemFiles)
    VECTOR(std::wstring, IncludeCustomFiles)
    MAP(std::wstring, std::wstring, PrivateProperties)
    MAP(std::wstring, std::wstring, ProtectedProperties)
    GETSET(bool, IsLogConfigIndependent, false)
    GETSET(bool, IsActionManagerIndependent, false)
    GETSET(bool, IsThreadManagerIndependent, false)
    GETSET(bool, IsJson, false)
    MAP(std::wstring, std::wstring, JsonAttributes)
    GETSET(std::wstring, InheritClass, L"")
    MAP(std::wstring, std::wstring, InheritClassAttributes)

    public:
        VPGEnumClass() : vcc::BaseObject(ObjectType::EnumClass) {}
        virtual ~VPGEnumClass() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            auto obj = std::make_shared<VPGEnumClass>(*this);
            obj->cloneProperties(this->_Properties);
            return obj;
        }
};
