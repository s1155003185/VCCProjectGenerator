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
    , Getset
    , GetsetSptr
    , GetsetSptrNull
    , Getcustom
    , GetcustomSptr
    , Setcustom
    , SetcustomSptr
    , Vector
    , VectorSptr
    , Set
    , SetSptr
    , Map
    , MapSptrR
    , OrderedMap
    , OrderedMapSptrR
    , ManagerSptr
    , ManagerSptrNull
    , ManagerSptrParent
    , Action
    , ActionWithArgSptr
};

class VPGEnumClassProperty : public BaseObject
{
    friend class VPGEnumClassReader;

    GETSET(VPGEnumClassPropertyType, PropertyType, VPGEnumClassPropertyType::NA);
    GETSET(std::wstring, Enum, L"");
    GETSET(int64_t, EnumValue, 0);
    GETSET(std::wstring, Macro, L"");
    GETSET(std::wstring, Type1, L"");
    GETSET(std::wstring, ArgumentName1, L"");
    GETSET(std::wstring, Type2, L"");
    GETSET(std::wstring, PropertyName, L"");
    GETSET(std::wstring, DefaultValue, L"");
    GETSET(std::wstring, Command, L"");

    // Macro
    GETSET(VPGEnumClassMacroType, MacroType, VPGEnumClassMacroType::NA);
    GETSET(VPGEnumClassPropertyAccessMode, AccessMode, VPGEnumClassPropertyAccessMode::ReadWrite);
    GETSET(bool, IsInherit, false);
    
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

        bool GetIsGeneralType() const { return !GetIsCollection(); }
        bool GetIsObject() const { return _Macro.find(L"(") != std::wstring::npos ? IsContain(_Macro.substr(0, _Macro.find(L"(")), L"_SPTR") : false; }
        bool GetIsVector() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Vector, VPGEnumClassMacroType::VectorSptr}, _MacroType); }
        bool GetIsMap() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Map, VPGEnumClassMacroType::MapSptrR}, _MacroType); }
        bool GetIsOrderedMap() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::OrderedMap, VPGEnumClassMacroType::OrderedMapSptrR}, _MacroType); }
        bool GetIsSet() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Set, VPGEnumClassMacroType::SetSptr}, _MacroType); }
        bool GetIsAction() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::Action, VPGEnumClassMacroType::ActionWithArgSptr}, _MacroType); }
        bool GetIsManager() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::ManagerSptr, VPGEnumClassMacroType::ManagerSptrNull, VPGEnumClassMacroType::ManagerSptrParent}, _MacroType); }
        bool GetIsCollection() const { return GetIsVector() || GetIsMap() || GetIsOrderedMap() || GetIsSet(); }

        bool GetIsInitializeInClassConstructorNeeded() const { return IsContain(std::vector<VPGEnumClassMacroType>{VPGEnumClassMacroType::GetsetSptr, VPGEnumClassMacroType::ManagerSptr}, _MacroType); }
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