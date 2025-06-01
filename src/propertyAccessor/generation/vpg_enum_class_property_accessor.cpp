#include "vpg_enum_class_property_accessor.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_enum_class_property.hpp"
#include "vpg_enum_class_type.hpp"

bool VPGEnumClassAttributePropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::IsAction:
            return obj->GetIsAction();
        case VPGEnumClassAttributeProperty::IsCollection:
            return obj->GetIsCollection();
        case VPGEnumClassAttributeProperty::IsCustom:
            return obj->GetIsCustom();
        case VPGEnumClassAttributeProperty::IsGeneralType:
            return obj->GetIsGeneralType();
        case VPGEnumClassAttributeProperty::IsHavingValidate:
            return obj->GetIsHavingValidate();
        case VPGEnumClassAttributeProperty::IsInherit:
            return obj->GetIsInherit();
        case VPGEnumClassAttributeProperty::IsInitializeInClassConstructorNeeded:
            return obj->GetIsInitializeInClassConstructorNeeded();
        case VPGEnumClassAttributeProperty::IsManager:
            return obj->GetIsManager();
        case VPGEnumClassAttributeProperty::IsMap:
            return obj->GetIsMap();
        case VPGEnumClassAttributeProperty::IsNoHistory:
            return obj->GetIsNoHistory();
        case VPGEnumClassAttributeProperty::IsNoJson:
            return obj->GetIsNoJson();
        case VPGEnumClassAttributeProperty::IsObject:
            return obj->GetIsObject();
        case VPGEnumClassAttributeProperty::IsOrderedMap:
            return obj->GetIsOrderedMap();
        case VPGEnumClassAttributeProperty::IsSet:
            return obj->GetIsSet();
        case VPGEnumClassAttributeProperty::IsType1Custom:
            return obj->GetIsType1Custom();
        case VPGEnumClassAttributeProperty::IsType2Custom:
            return obj->GetIsType2Custom();
        case VPGEnumClassAttributeProperty::IsVector:
            return obj->GetIsVector();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGEnumClassAttributePropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGEnumClassAttributePropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassAttributePropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::IsInherit:
            obj->SetIsInherit(value);
            break;
        case VPGEnumClassAttributeProperty::IsNoHistory:
            obj->SetIsNoHistory(value);
            break;
        case VPGEnumClassAttributeProperty::IsNoJson:
            obj->SetIsNoJson(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGEnumClassAttributePropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::AccessMode:
            return static_cast<long>(obj->GetAccessMode());
        case VPGEnumClassAttributeProperty::EnumValue:
            return obj->GetEnumValue();
        case VPGEnumClassAttributeProperty::MacroType:
            return static_cast<long>(obj->GetMacroType());
        case VPGEnumClassAttributeProperty::PropertyType:
            return static_cast<long>(obj->GetPropertyType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGEnumClassAttributePropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGEnumClassAttributePropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGEnumClassAttributePropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::AccessMode:
            obj->SetAccessMode(static_cast<VPGEnumClassAttributeAccessMode>(value));
            break;
        case VPGEnumClassAttributeProperty::EnumValue:
            obj->SetEnumValue(value);
            break;
        case VPGEnumClassAttributeProperty::MacroType:
            obj->SetMacroType(static_cast<VPGEnumClassMacroType>(value));
            break;
        case VPGEnumClassAttributeProperty::PropertyType:
            obj->SetPropertyType(static_cast<VPGEnumClassAttributeType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGEnumClassAttributePropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::ActionResultRedoClass:
            return obj->GetActionResultRedoClass();
        case VPGEnumClassAttributeProperty::ActionResultUndoClass:
            return obj->GetActionResultUndoClass();
        case VPGEnumClassAttributeProperty::Command:
            return obj->GetCommand();
        case VPGEnumClassAttributeProperty::DefaultValue:
            return obj->GetDefaultValue();
        case VPGEnumClassAttributeProperty::Enum:
            return obj->GetEnum();
        case VPGEnumClassAttributeProperty::Macro:
            return obj->GetMacro();
        case VPGEnumClassAttributeProperty::PropertyName:
            return obj->GetPropertyName();
        case VPGEnumClassAttributeProperty::Type1:
            return obj->GetType1();
        case VPGEnumClassAttributeProperty::Type2:
            return obj->GetType2();
        case VPGEnumClassAttributeProperty::Validate:
            return obj->GetValidate();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassAttributePropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            return obj->GetInitializePropertiesAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassAttributePropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGEnumClassAttributePropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::ActionResultRedoClass:
            obj->SetActionResultRedoClass(value);
            break;
        case VPGEnumClassAttributeProperty::ActionResultUndoClass:
            obj->SetActionResultUndoClass(value);
            break;
        case VPGEnumClassAttributeProperty::Command:
            obj->SetCommand(value);
            break;
        case VPGEnumClassAttributeProperty::DefaultValue:
            obj->SetDefaultValue(value);
            break;
        case VPGEnumClassAttributeProperty::Enum:
            obj->SetEnum(value);
            break;
        case VPGEnumClassAttributeProperty::Macro:
            obj->SetMacro(value);
            break;
        case VPGEnumClassAttributeProperty::PropertyName:
            obj->SetPropertyName(value);
            break;
        case VPGEnumClassAttributeProperty::Type1:
            obj->SetType1(value);
            break;
        case VPGEnumClassAttributeProperty::Type2:
            obj->SetType2(value);
            break;
        case VPGEnumClassAttributeProperty::Validate:
            obj->SetValidate(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            if (index > -1)
                obj->SetInitializePropertiesAtIndex(index, value);
            else
                obj->InsertInitializeProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            if (index > -1)
                obj->InsertInitializePropertiesAtIndex(index, value);
            else
                obj->InsertInitializeProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

size_t VPGEnumClassAttributePropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            return obj->GetInitializeProperties().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGEnumClassAttributePropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGEnumClassAttributePropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassAttributePropertyAccessor::_Remove(const int64_t &objectProperty, const void *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveInitializeProperties(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_RemoveObject(const int64_t &objectProperty, const vcc::IObject */*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            obj->RemoveInitializePropertiesAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_Clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            obj->ClearInitializeProperties();
            break;
        default:
            assert(false);
        }
    CATCH
}

bool VPGEnumClassPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IsActionManagerIndependent:
            return obj->GetIsActionManagerIndependent();
        case VPGEnumClassProperty::IsJson:
            return obj->GetIsJson();
        case VPGEnumClassProperty::IsLogConfigIndependent:
            return obj->GetIsLogConfigIndependent();
        case VPGEnumClassProperty::IsThreadManagerIndependent:
            return obj->GetIsThreadManagerIndependent();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGEnumClassPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGEnumClassPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IsActionManagerIndependent:
            obj->SetIsActionManagerIndependent(value);
            break;
        case VPGEnumClassProperty::IsJson:
            obj->SetIsJson(value);
            break;
        case VPGEnumClassProperty::IsLogConfigIndependent:
            obj->SetIsLogConfigIndependent(value);
            break;
        case VPGEnumClassProperty::IsThreadManagerIndependent:
            obj->SetIsThreadManagerIndependent(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGEnumClassPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Type:
            return static_cast<long>(obj->GetType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGEnumClassPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGEnumClassPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGEnumClassPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Type:
            obj->SetType(static_cast<VPGEnumClassType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGEnumClassPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Command:
            return obj->GetCommand();
        case VPGEnumClassProperty::InheritClass:
            return obj->GetInheritClass();
        case VPGEnumClassProperty::Name:
            return obj->GetName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            return obj->GetIncludeCustomFilesAtIndex(index);
        case VPGEnumClassProperty::IncludeSystemFiles:
            return obj->GetIncludeSystemFilesAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void *key) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        assert(key != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->GetInheritClassAttributesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->GetJsonAttributesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->GetPrivatePropertiesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->GetProtectedPropertiesAtKey(keyPtr);
        }
        default:
            assert(false);
        }
    CATCH
    return L"";
}

void VPGEnumClassPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Command:
            obj->SetCommand(value);
            break;
        case VPGEnumClassProperty::InheritClass:
            obj->SetInheritClass(value);
            break;
        case VPGEnumClassProperty::Name:
            obj->SetName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            if (index > -1)
                obj->SetIncludeCustomFilesAtIndex(index, value);
            else
                obj->InsertIncludeCustomFiles(value);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            if (index > -1)
                obj->SetIncludeSystemFilesAtIndex(index, value);
            else
                obj->InsertIncludeSystemFiles(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &value, const void *key)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        assert(key != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->IsInheritClassAttributesContainKey(keyPtr))
                obj->SetInheritClassAttributesAtKey(keyPtr, value);
            else
                obj->InsertInheritClassAttributesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->IsJsonAttributesContainKey(keyPtr))
                obj->SetJsonAttributesAtKey(keyPtr, value);
            else
                obj->InsertJsonAttributesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->IsPrivatePropertiesContainKey(keyPtr))
                obj->SetPrivatePropertiesAtKey(keyPtr, value);
            else
                obj->InsertPrivatePropertiesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->IsProtectedPropertiesContainKey(keyPtr))
                obj->SetProtectedPropertiesAtKey(keyPtr, value);
            else
                obj->InsertProtectedPropertiesAtKey(keyPtr, value);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            if (index > -1)
                obj->InsertIncludeCustomFilesAtIndex(index, value);
            else
                obj->InsertIncludeCustomFiles(value);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            if (index > -1)
                obj->InsertIncludeSystemFilesAtIndex(index, value);
            else
                obj->InsertIncludeSystemFiles(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return std::static_pointer_cast<vcc::IObject>(obj->GetPropertiesAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGEnumClassPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            if (index > -1)
                obj->SetPropertiesAtIndex(index, std::static_pointer_cast<VPGEnumClassAttribute>(value));
            else
                obj->InsertProperties(std::static_pointer_cast<VPGEnumClassAttribute>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            if (index > -1)
                obj->InsertPropertiesAtIndex(index, std::static_pointer_cast<VPGEnumClassAttribute>(value));
            else
                obj->InsertProperties(std::static_pointer_cast<VPGEnumClassAttribute>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return std::static_pointer_cast<vcc::IObject>(obj->ClonePropertiesAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGEnumClassPropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return obj->GetProperties().size();
        case VPGEnumClassProperty::IncludeSystemFiles:
            return obj->GetIncludeSystemFiles().size();
        case VPGEnumClassProperty::IncludeCustomFiles:
            return obj->GetIncludeCustomFiles().size();
        case VPGEnumClassProperty::PrivateProperties:
            return obj->GetPrivateProperties().size();
        case VPGEnumClassProperty::ProtectedProperties:
            return obj->GetProtectedProperties().size();
        case VPGEnumClassProperty::JsonAttributes:
            return obj->GetJsonAttributes().size();
        case VPGEnumClassProperty::InheritClassAttributes:
            return obj->GetInheritClassAttributes().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGEnumClassPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes:
            return obj->GetInheritClassAttributesVoidKeys();
        case VPGEnumClassProperty::JsonAttributes:
            return obj->GetJsonAttributesVoidKeys();
        case VPGEnumClassProperty::PrivateProperties:
            return obj->GetPrivatePropertiesVoidKeys();
        case VPGEnumClassProperty::ProtectedProperties:
            return obj->GetProtectedPropertiesVoidKeys();
        default:
            assert(false);
        }
    CATCH
    return result;
}

bool VPGEnumClassPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void *key) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        assert(key != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->IsInheritClassAttributesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->IsJsonAttributesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->IsPrivatePropertiesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->IsProtectedPropertiesContainKey(keyPtr);
        }
        default:
            assert(false);
        }
    CATCH
    return false;
}

void VPGEnumClassPropertyAccessor::_Remove(const int64_t &objectProperty, const void *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveIncludeCustomFiles(valuePtr);
            break;
        }
        case VPGEnumClassProperty::IncludeSystemFiles: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveIncludeSystemFiles(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_RemoveObject(const int64_t &objectProperty, const vcc::IObject *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            obj->RemoveProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            obj->RemoveIncludeCustomFilesAtIndex(index);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            obj->RemoveIncludeSystemFilesAtIndex(index);
            break;
        case VPGEnumClassProperty::Properties:
            obj->RemovePropertiesAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void *key)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        assert(key != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->RemoveInheritClassAttributesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->RemoveJsonAttributesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->RemovePrivatePropertiesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->RemoveProtectedPropertiesAtKey(keyPtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_Clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            obj->ClearProperties();
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            obj->ClearIncludeSystemFiles();
            break;
        case VPGEnumClassProperty::IncludeCustomFiles:
            obj->ClearIncludeCustomFiles();
            break;
        case VPGEnumClassProperty::PrivateProperties:
            obj->ClearPrivateProperties();
            break;
        case VPGEnumClassProperty::ProtectedProperties:
            obj->ClearProtectedProperties();
            break;
        case VPGEnumClassProperty::JsonAttributes:
            obj->ClearJsonAttributes();
            break;
        case VPGEnumClassProperty::InheritClassAttributes:
            obj->ClearInheritClassAttributes();
            break;
        default:
            assert(false);
        }
    CATCH
}
