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

bool VPGEnumClassAttributePropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::IsAction:
            return obj->getIsAction();
        case VPGEnumClassAttributeProperty::IsCollection:
            return obj->getIsCollection();
        case VPGEnumClassAttributeProperty::IsCustom:
            return obj->getIsCustom();
        case VPGEnumClassAttributeProperty::IsGeneralType:
            return obj->getIsGeneralType();
        case VPGEnumClassAttributeProperty::IsHavingValidate:
            return obj->getIsHavingValidate();
        case VPGEnumClassAttributeProperty::IsInherit:
            return obj->getIsInherit();
        case VPGEnumClassAttributeProperty::IsInitializeInClassConstructorNeeded:
            return obj->getIsInitializeInClassConstructorNeeded();
        case VPGEnumClassAttributeProperty::IsManager:
            return obj->getIsManager();
        case VPGEnumClassAttributeProperty::IsMap:
            return obj->getIsMap();
        case VPGEnumClassAttributeProperty::IsNoHistory:
            return obj->getIsNoHistory();
        case VPGEnumClassAttributeProperty::IsNoJson:
            return obj->getIsNoJson();
        case VPGEnumClassAttributeProperty::IsObject:
            return obj->getIsObject();
        case VPGEnumClassAttributeProperty::IsOrderedMap:
            return obj->getIsOrderedMap();
        case VPGEnumClassAttributeProperty::IsSet:
            return obj->getIsSet();
        case VPGEnumClassAttributeProperty::IsType1Custom:
            return obj->getIsType1Custom();
        case VPGEnumClassAttributeProperty::IsType2Custom:
            return obj->getIsType2Custom();
        case VPGEnumClassAttributeProperty::IsVector:
            return obj->getIsVector();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGEnumClassAttributePropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGEnumClassAttributePropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassAttributePropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::IsInherit:
            obj->setIsInherit(value);
            break;
        case VPGEnumClassAttributeProperty::IsNoHistory:
            obj->setIsNoHistory(value);
            break;
        case VPGEnumClassAttributeProperty::IsNoJson:
            obj->setIsNoJson(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGEnumClassAttributePropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::AccessMode:
            return static_cast<long>(obj->getAccessMode());
        case VPGEnumClassAttributeProperty::EnumValue:
            return obj->getEnumValue();
        case VPGEnumClassAttributeProperty::MacroType:
            return static_cast<long>(obj->getMacroType());
        case VPGEnumClassAttributeProperty::PropertyType:
            return static_cast<long>(obj->getPropertyType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGEnumClassAttributePropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGEnumClassAttributePropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGEnumClassAttributePropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::AccessMode:
            obj->setAccessMode(static_cast<VPGEnumClassAttributeAccessMode>(value));
            break;
        case VPGEnumClassAttributeProperty::EnumValue:
            obj->setEnumValue(value);
            break;
        case VPGEnumClassAttributeProperty::MacroType:
            obj->setMacroType(static_cast<VPGEnumClassMacroType>(value));
            break;
        case VPGEnumClassAttributeProperty::PropertyType:
            obj->setPropertyType(static_cast<VPGEnumClassAttributeType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGEnumClassAttributePropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::ActionResultRedoClass:
            return obj->getActionResultRedoClass();
        case VPGEnumClassAttributeProperty::ActionResultUndoClass:
            return obj->getActionResultUndoClass();
        case VPGEnumClassAttributeProperty::Command:
            return obj->getCommand();
        case VPGEnumClassAttributeProperty::DefaultValue:
            return obj->getDefaultValue();
        case VPGEnumClassAttributeProperty::Enum:
            return obj->getEnum();
        case VPGEnumClassAttributeProperty::Macro:
            return obj->getMacro();
        case VPGEnumClassAttributeProperty::PropertyName:
            return obj->getPropertyName();
        case VPGEnumClassAttributeProperty::Type1:
            return obj->getType1();
        case VPGEnumClassAttributeProperty::Type2:
            return obj->getType2();
        case VPGEnumClassAttributeProperty::Validate:
            return obj->getValidate();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassAttributePropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            return obj->getInitializePropertiesAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassAttributePropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGEnumClassAttributePropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::ActionResultRedoClass:
            obj->setActionResultRedoClass(value);
            break;
        case VPGEnumClassAttributeProperty::ActionResultUndoClass:
            obj->setActionResultUndoClass(value);
            break;
        case VPGEnumClassAttributeProperty::Command:
            obj->setCommand(value);
            break;
        case VPGEnumClassAttributeProperty::DefaultValue:
            obj->setDefaultValue(value);
            break;
        case VPGEnumClassAttributeProperty::Enum:
            obj->setEnum(value);
            break;
        case VPGEnumClassAttributeProperty::Macro:
            obj->setMacro(value);
            break;
        case VPGEnumClassAttributeProperty::PropertyName:
            obj->setPropertyName(value);
            break;
        case VPGEnumClassAttributeProperty::Type1:
            obj->setType1(value);
            break;
        case VPGEnumClassAttributeProperty::Type2:
            obj->setType2(value);
            break;
        case VPGEnumClassAttributeProperty::Validate:
            obj->setValidate(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            if (index > -1)
                obj->setInitializePropertiesAtIndex(index, value);
            else
                obj->insertInitializeProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            if (index > -1)
                obj->insertInitializePropertiesAtIndex(index, value);
            else
                obj->insertInitializeProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

size_t VPGEnumClassAttributePropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            return obj->getInitializeProperties().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGEnumClassAttributePropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGEnumClassAttributePropertyAccessor::_isContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassAttributePropertyAccessor::_remove(const int64_t &objectProperty, const void *value)
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
            obj->removeInitializeProperties(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject */*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            obj->removeInitializePropertiesAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassAttributePropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClassAttribute>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassAttributeProperty>(objectProperty))
        {
        case VPGEnumClassAttributeProperty::InitializeProperties:
            obj->clearInitializeProperties();
            break;
        default:
            assert(false);
        }
    CATCH
}

bool VPGEnumClassPropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IsActionManagerIndependent:
            return obj->getIsActionManagerIndependent();
        case VPGEnumClassProperty::IsJson:
            return obj->getIsJson();
        case VPGEnumClassProperty::IsLogConfigIndependent:
            return obj->getIsLogConfigIndependent();
        case VPGEnumClassProperty::IsThreadManagerIndependent:
            return obj->getIsThreadManagerIndependent();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGEnumClassPropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGEnumClassPropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGEnumClassPropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IsActionManagerIndependent:
            obj->setIsActionManagerIndependent(value);
            break;
        case VPGEnumClassProperty::IsJson:
            obj->setIsJson(value);
            break;
        case VPGEnumClassProperty::IsLogConfigIndependent:
            obj->setIsLogConfigIndependent(value);
            break;
        case VPGEnumClassProperty::IsThreadManagerIndependent:
            obj->setIsThreadManagerIndependent(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGEnumClassPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Type:
            return static_cast<long>(obj->getType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGEnumClassPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGEnumClassPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGEnumClassPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Type:
            obj->setType(static_cast<VPGEnumClassType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGEnumClassPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Command:
            return obj->getCommand();
        case VPGEnumClassProperty::InheritClass:
            return obj->getInheritClass();
        case VPGEnumClassProperty::Name:
            return obj->getName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            return obj->getIncludeCustomFilesAtIndex(index);
        case VPGEnumClassProperty::IncludeSystemFiles:
            return obj->getIncludeSystemFilesAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGEnumClassPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void *key) const
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
            return obj->getInheritClassAttributesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->getJsonAttributesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->getPrivatePropertiesAtKey(keyPtr);
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->getProtectedPropertiesAtKey(keyPtr);
        }
        default:
            assert(false);
        }
    CATCH
    return L"";
}

void VPGEnumClassPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Command:
            obj->setCommand(value);
            break;
        case VPGEnumClassProperty::InheritClass:
            obj->setInheritClass(value);
            break;
        case VPGEnumClassProperty::Name:
            obj->setName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            if (index > -1)
                obj->setIncludeCustomFilesAtIndex(index, value);
            else
                obj->insertIncludeCustomFiles(value);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            if (index > -1)
                obj->setIncludeSystemFilesAtIndex(index, value);
            else
                obj->insertIncludeSystemFiles(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &value, const void *key)
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
            if (obj->isInheritClassAttributesContainKey(keyPtr))
                obj->setInheritClassAttributesAtKey(keyPtr, value);
            else
                obj->insertInheritClassAttributesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->isJsonAttributesContainKey(keyPtr))
                obj->setJsonAttributesAtKey(keyPtr, value);
            else
                obj->insertJsonAttributesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->isPrivatePropertiesContainKey(keyPtr))
                obj->setPrivatePropertiesAtKey(keyPtr, value);
            else
                obj->insertPrivatePropertiesAtKey(keyPtr, value);
            break;
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            if (obj->isProtectedPropertiesContainKey(keyPtr))
                obj->setProtectedPropertiesAtKey(keyPtr, value);
            else
                obj->insertProtectedPropertiesAtKey(keyPtr, value);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            if (index > -1)
                obj->insertIncludeCustomFilesAtIndex(index, value);
            else
                obj->insertIncludeCustomFiles(value);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            if (index > -1)
                obj->insertIncludeSystemFilesAtIndex(index, value);
            else
                obj->insertIncludeSystemFiles(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return std::static_pointer_cast<vcc::IObject>(obj->getPropertiesAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGEnumClassPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            if (index > -1)
                obj->setPropertiesAtIndex(index, std::static_pointer_cast<VPGEnumClassAttribute>(value));
            else
                obj->insertProperties(std::static_pointer_cast<VPGEnumClassAttribute>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGEnumClassPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            if (index > -1)
                obj->insertPropertiesAtIndex(index, std::static_pointer_cast<VPGEnumClassAttribute>(value));
            else
                obj->insertProperties(std::static_pointer_cast<VPGEnumClassAttribute>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return std::static_pointer_cast<vcc::IObject>(obj->clonePropertiesAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGEnumClassPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGEnumClassPropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            return obj->getProperties().size();
        case VPGEnumClassProperty::IncludeSystemFiles:
            return obj->getIncludeSystemFiles().size();
        case VPGEnumClassProperty::IncludeCustomFiles:
            return obj->getIncludeCustomFiles().size();
        case VPGEnumClassProperty::PrivateProperties:
            return obj->getPrivateProperties().size();
        case VPGEnumClassProperty::ProtectedProperties:
            return obj->getProtectedProperties().size();
        case VPGEnumClassProperty::JsonAttributes:
            return obj->getJsonAttributes().size();
        case VPGEnumClassProperty::InheritClassAttributes:
            return obj->getInheritClassAttributes().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGEnumClassPropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::InheritClassAttributes:
            return obj->getInheritClassAttributesVoidKeys();
        case VPGEnumClassProperty::JsonAttributes:
            return obj->getJsonAttributesVoidKeys();
        case VPGEnumClassProperty::PrivateProperties:
            return obj->getPrivatePropertiesVoidKeys();
        case VPGEnumClassProperty::ProtectedProperties:
            return obj->getProtectedPropertiesVoidKeys();
        default:
            assert(false);
        }
    CATCH
    return result;
}

bool VPGEnumClassPropertyAccessor::_isContainKey(const int64_t &objectProperty, const void *key) const
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
            return obj->isInheritClassAttributesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->isJsonAttributesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->isPrivatePropertiesContainKey(keyPtr);
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->isProtectedPropertiesContainKey(keyPtr);
        }
        default:
            assert(false);
        }
    CATCH
    return false;
}

void VPGEnumClassPropertyAccessor::_remove(const int64_t &objectProperty, const void *value)
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
            obj->removeIncludeCustomFiles(valuePtr);
            break;
        }
        case VPGEnumClassProperty::IncludeSystemFiles: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->removeIncludeSystemFiles(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            obj->removeProperties(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::IncludeCustomFiles:
            obj->removeIncludeCustomFilesAtIndex(index);
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            obj->removeIncludeSystemFilesAtIndex(index);
            break;
        case VPGEnumClassProperty::Properties:
            obj->removePropertiesAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void *key)
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
            obj->removeInheritClassAttributesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::JsonAttributes: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->removeJsonAttributesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::PrivateProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->removePrivatePropertiesAtKey(keyPtr);
            break;
        }
        case VPGEnumClassProperty::ProtectedProperties: {
            auto keyPtr = static_cast<const wchar_t *>(key);
            assert(keyPtr != nullptr);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->removeProtectedPropertiesAtKey(keyPtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGEnumClassPropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGEnumClass>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGEnumClassProperty>(objectProperty))
        {
        case VPGEnumClassProperty::Properties:
            obj->clearProperties();
            break;
        case VPGEnumClassProperty::IncludeSystemFiles:
            obj->clearIncludeSystemFiles();
            break;
        case VPGEnumClassProperty::IncludeCustomFiles:
            obj->clearIncludeCustomFiles();
            break;
        case VPGEnumClassProperty::PrivateProperties:
            obj->clearPrivateProperties();
            break;
        case VPGEnumClassProperty::ProtectedProperties:
            obj->clearProtectedProperties();
            break;
        case VPGEnumClassProperty::JsonAttributes:
            obj->clearJsonAttributes();
            break;
        case VPGEnumClassProperty::InheritClassAttributes:
            obj->clearInheritClassAttributes();
            break;
        default:
            assert(false);
        }
    CATCH
}
