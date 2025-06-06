#pragma once

/*
@@Private {
    "Properties": {
        "_DefaultValue": "std::wstring=L\"\"",
        "_AccessMode": "VPGEnumClassAttributeAccessMode=VPGEnumClassAttributeAccessMode::ReadWrite"
    }
}
*/
enum class VPGEnumClassAttributeProperty
{
    PropertyType // GETSET(VPGEnumClassAttributeType, PropertyType, VPGEnumClassAttributeType::NA);
    , Enum // GETSET(std::wstring, Enum, L"")
    , EnumValue // GETSET(int64_t, EnumValue, 0)
    , Macro // GETSET(std::wstring, Macro, L"")
    , MacroType // GETSET(VPGEnumClassMacroType, MacroType, VPGEnumClassMacroType::NA);
    , Type1 // GETSET(std::wstring, Type1, L"")
    , IsType1Custom // GETCUSTOM(bool, IsType1Custom, return vcc::IsCapital(vcc::SplitString(GetType1(), {L"::"}).back());)
    , Type2 // GETSET(std::wstring, Type2, L"")
    , IsType2Custom // GETCUSTOM(bool, IsType2Custom, return vcc::IsCapital(vcc::SplitString(GetType2(), {L"::"}).back());)
    , PropertyName // GETSET(std::wstring, PropertyName, L"");
    , Validate // GETSET(std::wstring, Validate, L"")
    , DefaultValue // GETCUSTOM(std::wstring, DefaultValue, if (_InitializeProperties.empty()) return _DefaultValue; return vcc::Concat(_InitializeProperties, L", ");) SETCUSTOM(DefaultValue, std::wstring, _DefaultValue = value;)
    , Command // GETSET(std::wstring, Command, L"")
    
    // Properties
    , IsGeneralType // GETCUSTOM(bool, IsGeneralType, return !GetIsCollection();)
    , IsCustom // GETCUSTOM(bool, IsCustom, return vcc::IsContain(GetMacro(), L"(") ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"CUSTOM") : false;)
    , IsObject // GETCUSTOM(bool, IsObject, return vcc::IsContain(GetMacro(), L"(") ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"_SPTR") : false;)
    , IsVector // GETCUSTOM(bool, IsVector, return vcc::IsStartWith(_Macro, L"VECTOR");)
    , IsMap // GETCUSTOM(bool, IsMap, return vcc::IsStartWith(_Macro, L"MAP");)
    , IsOrderedMap // GETCUSTOM(bool, IsOrderedMap, return vcc::IsStartWith(_Macro, L"ORDERED_MAP");)
    , IsSet // GETCUSTOM(bool, IsSet, return vcc::IsContain(std::vector<VPGEnumClassMacroType>{ VPGEnumClassMacroType::Set, VPGEnumClassMacroType::SetSptr, VPGEnumClassMacroType::SetValidate, VPGEnumClassMacroType::SetValidateSptr }, _MacroType);)
    , IsAction // GETCUSTOM(bool, IsAction, return vcc::IsStartWith(_Macro, L"ACTION");)
    , IsManager // GETCUSTOM(bool, IsManager, return vcc::IsStartWith(_Macro, L"MANAGER");)
    , IsCollection // GETCUSTOM(bool, IsCollection, return GetIsVector() || GetIsMap() || GetIsOrderedMap() || GetIsSet();)
    , IsHavingValidate // GETCUSTOM(bool, IsHavingValidate, return vcc::IsContain(GetMacro(), L"(") ? vcc::IsContain(_Macro.substr(0, _Macro.find(L"(")), L"VALIDATE") : false;)

    // Macro
    , AccessMode /* GETCUSTOM(VPGEnumClassAttributeAccessMode, AccessMode, 
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
        SETCUSTOM(AccessMode, VPGEnumClassAttributeAccessMode, _AccessMode = value;) */
    , IsInherit // GETSET(bool, IsInherit, false);
    
    // Properties
    , InitializeProperties // VECTOR(std::wstring, InitializeProperties)

    // Action
    , IsNoHistory // GETSET(bool, IsNoHistory, false);
    , ActionResultRedoClass // GETSET(std::wstring, ActionResultRedoClass, L"vcc::OperationResult")
    , ActionResultUndoClass // GETSET(std::wstring, ActionResultUndoClass, L"vcc::OperationResult")

    // Json
    , IsNoJson // GETSET(bool, IsNoJson, false)

    // Generator Behavior
    , IsInitializeInClassConstructorNeeded /* GETCUSTOM(bool, IsInitializeInClassConstructorNeeded, 
                                                return vcc::IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::GetsetSptr, VPGEnumClassMacroType::ManagerSptr}, _MacroType)
                                                    || (!vcc::IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Getcustom, VPGEnumClassMacroType::GetcustomSptr}, _MacroType) && !_InitializeProperties.empty());)*/
};

enum class VPGEnumClassProperty
{
    Type // GETSET(VPGEnumClassType, Type, VPGEnumClassType::Object)
    , Name // GETSET(std::wstring, Name, L"")
    , Command // GETSET(std::wstring, Command, L"")
    , Properties // VECTOR_SPTR(VPGEnumClassAttribute, Properties)

    // Files
    , IncludeSystemFiles // VECTOR(std::wstring, IncludeSystemFiles)
    , IncludeCustomFiles // VECTOR(std::wstring, IncludeCustomFiles)

    // Properties
    , PrivateProperties // MAP(std::wstring, std::wstring, PrivateProperties)
    , ProtectedProperties // MAP(std::wstring, std::wstring, ProtectedProperties)

    // Log
    , IsLogConfigIndependent // GETSET(bool, IsLogConfigIndependent, false)

    // Action
    , IsActionManagerIndependent // GETSET(bool, IsActionManagerIndependent, false)

    // Thread
    , IsThreadManagerIndependent // GETSET(bool, IsThreadManagerIndependent, false)

    // Json
    , IsJson // GETSET(bool, IsJson, false)
    , JsonAttributes // MAP(std::wstring, std::wstring, JsonAttributes)
    , InheritClass // GETSET(std::wstring, InheritClass, L"")
    , InheritClassAttributes // MAP(std::wstring, std::wstring, InheritClassAttributes)
};
