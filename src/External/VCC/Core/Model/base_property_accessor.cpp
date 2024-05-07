#include "base_property_accessor.hpp"

#include <shared_mutex>
#include <string>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "lock_type.hpp"

namespace vcc
{       
    #define THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Object Property Type " + std::to_wstring(objectProperty) + L" not found");
    #define LOCK_BEGIN switch(lockType) \
        {\
            case LockType::NoLock:\
                break;\
            case LockType::ReadLock:\
                ReadLock();\
                break;\
            case LockType::WriteLock:\
            case LockType::ReadWriteLock:\
                WriteLock();\
                break;\
            default:\
                assert(false);\
        }\
        TRY_CATCH() {

    #define LOCK_END Unlock(); }

    void BasePropertyAccessor::ReadLock() const { _Mutex->lock_shared(); }
    void BasePropertyAccessor::WriteLock() const { _Mutex->lock(); }
    void BasePropertyAccessor::ReadWriteLock() const { WriteLock(); }
    void BasePropertyAccessor::Unlock() const { _Mutex->unlock(); }
    
    bool BasePropertyAccessor::_ReadBool(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return false; }
    bool BasePropertyAccessor::_ReadBool(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return false; }
    void BasePropertyAccessor::_WriteBool(const size_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteBool(const size_t &objectProperty, const bool &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertBool(const size_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertBool(const size_t &objectProperty, const bool &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // char, byte, int8_t
    char BasePropertyAccessor::_ReadChar(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return '\0'; }
    char BasePropertyAccessor::_ReadChar(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return '\0'; }
    void BasePropertyAccessor::_WriteChar(const size_t &objectProperty, const char &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteChar(const size_t &objectProperty, const char &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertChar(const size_t &objectProperty, const char &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertChar(const size_t &objectProperty, const char &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // wchar_t
    wchar_t BasePropertyAccessor::_ReadWchar(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return L'\0'; }
    wchar_t BasePropertyAccessor::_ReadWchar(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return L'\0'; }
    void BasePropertyAccessor::_WriteWchar(const size_t &objectProperty, const wchar_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteWchar(const size_t &objectProperty, const wchar_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertWchar(const size_t &objectProperty, const wchar_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertWchar(const size_t &objectProperty, const wchar_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // short
    short BasePropertyAccessor::_ReadShort(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    short BasePropertyAccessor::_ReadShort(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0;}
    void BasePropertyAccessor::_WriteShort(const size_t &objectProperty, const short &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteShort(const size_t &objectProperty, const short &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertShort(const size_t &objectProperty, const short &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertShort(const size_t &objectProperty, const short &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // uint16_t
    uint16_t BasePropertyAccessor::_ReadUnsignedShort(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    uint16_t BasePropertyAccessor::_ReadUnsignedShort(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteUnsignedShort(const size_t &objectProperty, const uint16_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteUnsignedShort(const size_t &objectProperty, const uint16_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedShort(const size_t &objectProperty, const uint16_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedShort(const size_t &objectProperty, const uint16_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // int, int32_t
    int BasePropertyAccessor::_ReadInt(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    int BasePropertyAccessor::_ReadInt(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteInt(const size_t &objectProperty, const int &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteInt(const size_t &objectProperty, const int &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertInt(const size_t &objectProperty, const int &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertInt(const size_t &objectProperty, const int &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // uint32_t
    uint32_t BasePropertyAccessor::_ReadUnsignedInt(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    uint32_t BasePropertyAccessor::_ReadUnsignedInt(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteUnsignedInt(const size_t &objectProperty, const uint32_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteUnsignedInt(const size_t &objectProperty, const uint32_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedInt(const size_t &objectProperty, const uint32_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedInt(const size_t &objectProperty, const uint32_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // long, int64_t, time_t, enum
    long BasePropertyAccessor::_ReadLong(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    long BasePropertyAccessor::_ReadLong(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteLong(const size_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteLong(const size_t &objectProperty, const long &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertLong(const size_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertLong(const size_t &objectProperty, const long &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // size_t
    size_t BasePropertyAccessor::_ReadUnsignedLong(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    size_t BasePropertyAccessor::_ReadUnsignedLong(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteUnsignedLong(const size_t &objectProperty, const size_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteUnsignedLong(const size_t &objectProperty, const size_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedLong(const size_t &objectProperty, const size_t &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertUnsignedLong(const size_t &objectProperty, const size_t &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // float
    float BasePropertyAccessor::_ReadFloat(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    float BasePropertyAccessor::_ReadFloat(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteFloat(const size_t &objectProperty, const float &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteFloat(const size_t &objectProperty, const float &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertFloat(const size_t &objectProperty, const float &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertFloat(const size_t &objectProperty, const float &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // double
    double BasePropertyAccessor::_ReadDouble(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    double BasePropertyAccessor::_ReadDouble(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0; }
    void BasePropertyAccessor::_WriteDouble(const size_t &objectProperty, const double &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteDouble(const size_t &objectProperty, const double &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertDouble(const size_t &objectProperty, const double &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertDouble(const size_t &objectProperty, const double &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // string
    std::string BasePropertyAccessor::_ReadString(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return ""; }
    std::string BasePropertyAccessor::_ReadString(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return ""; }
    void BasePropertyAccessor::_WriteString(const size_t &objectProperty, const std::string &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteString(const size_t &objectProperty, const std::string &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertString(const size_t &objectProperty, const std::string &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertString(const size_t &objectProperty, const std::string &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // wstring
    std::wstring BasePropertyAccessor::_ReadWstring(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return L""; }
    std::wstring BasePropertyAccessor::_ReadWstring(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return L""; }
    void BasePropertyAccessor::_WriteWstring(const size_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteWstring(const size_t &objectProperty, const std::wstring &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertWstring(const size_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertWstring(const size_t &objectProperty, const std::wstring &/*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // object
    std::shared_ptr<IObject> BasePropertyAccessor::_ReadObject(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return nullptr; }
    std::shared_ptr<IObject> BasePropertyAccessor::_ReadObject(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return nullptr; }
    void BasePropertyAccessor::_WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> /*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertObject(const size_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_InsertObject(const size_t &objectProperty, std::shared_ptr<IObject> /*value*/, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    std::shared_ptr<IObject> BasePropertyAccessor::_CloneObject(const size_t &objectProperty, const int64_t &/*index*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return nullptr; }
    std::shared_ptr<IObject> BasePropertyAccessor::_CloneObject(const size_t &objectProperty, const ITypeUnion */*key*/) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return nullptr; }
    void BasePropertyAccessor::_ClearObject(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    // vector, set, map
    size_t BasePropertyAccessor::_GetContainerCount(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND return 0;}
    void BasePropertyAccessor::_RemoveContainerElement(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }
    void BasePropertyAccessor::_ClearContainer(const size_t &objectProperty) const { THROW_EXCEPTION_MSG_FOR_OBJECT_PROPERTY_NOT_FOUND }

    
    // bool
    bool BasePropertyAccessor::ReadBool(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { bool result = false; LOCK_BEGIN result = _ReadBool(objectProperty, index); LOCK_END return result; }
    bool BasePropertyAccessor::ReadBool(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { bool result = false; LOCK_BEGIN result = _ReadBool(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index) const { LOCK_BEGIN _WriteBool(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteBool(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index) const { LOCK_BEGIN _InsertBool(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertBool(objectProperty, value, key); LOCK_END }

    // char, byte, int8_t
    char BasePropertyAccessor::ReadChar(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { char result = L'\0'; LOCK_BEGIN result = _ReadChar(objectProperty, index); LOCK_END return result; }
    char BasePropertyAccessor::ReadChar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { char result = L'\0'; LOCK_BEGIN result = _ReadChar(objectProperty, key); LOCK_END  return result; }
    void BasePropertyAccessor::WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index) const { LOCK_BEGIN _WriteChar(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteChar(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index) const { LOCK_BEGIN _InsertChar(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertChar(objectProperty, value, key); LOCK_END }

    // wchar_t
    wchar_t BasePropertyAccessor::ReadWchar(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { wchar_t result = L'\0'; LOCK_BEGIN result = _ReadWchar(objectProperty, index); LOCK_END return result; }
    wchar_t BasePropertyAccessor::ReadWchar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { wchar_t result = L'\0'; LOCK_BEGIN result = _ReadWchar(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index) const { LOCK_BEGIN _WriteWchar(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteWchar(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index) const { LOCK_BEGIN _InsertWchar(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertWchar(objectProperty, value, key); LOCK_END }

    // short
    short BasePropertyAccessor::ReadShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { short result = 0; LOCK_BEGIN result = _ReadShort(objectProperty, index); LOCK_END return result; }
    short BasePropertyAccessor::ReadShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { short result = 0;  LOCK_BEGIN result = _ReadShort(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index) const { LOCK_BEGIN _WriteShort(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteShort(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index) const { LOCK_BEGIN _InsertShort(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertShort(objectProperty, value, key); LOCK_END }

    // uint16_t
    uint16_t BasePropertyAccessor::ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { uint16_t result = 0; LOCK_BEGIN result = _ReadUnsignedShort(objectProperty, index); LOCK_END return result; }
    uint16_t BasePropertyAccessor::ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { uint16_t result = 0; LOCK_BEGIN result = _ReadUnsignedShort(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index) const { LOCK_BEGIN _WriteUnsignedShort(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteUnsignedShort(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index) const { LOCK_BEGIN _InsertUnsignedShort(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertUnsignedShort(objectProperty, value, key); LOCK_END }

    // int, int32_t
    int BasePropertyAccessor::ReadInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { int result = 0; LOCK_BEGIN result = _ReadInt(objectProperty, index); LOCK_END return result; }
    int BasePropertyAccessor::ReadInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { int result = 0; LOCK_BEGIN result = _ReadInt(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index) const { LOCK_BEGIN _WriteInt(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteInt(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index) const { LOCK_BEGIN _InsertInt(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertInt(objectProperty, value, key); LOCK_END }

    // uint32_t
    uint32_t BasePropertyAccessor::ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { uint32_t result = 0; LOCK_BEGIN result = _ReadUnsignedInt(objectProperty, index); LOCK_END return result; }
    uint32_t BasePropertyAccessor::ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { uint32_t result = 0; LOCK_BEGIN result = _ReadUnsignedInt(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index) const { LOCK_BEGIN _WriteUnsignedInt(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteUnsignedInt(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index) const { LOCK_BEGIN _InsertUnsignedInt(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertUnsignedInt(objectProperty, value, key); LOCK_END }

    // long, int64_t, time_t, enum
    long BasePropertyAccessor::ReadLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { long result = 0; LOCK_BEGIN result = _ReadLong(objectProperty, index); LOCK_END return result; }
    long BasePropertyAccessor::ReadLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { long result = 0; LOCK_BEGIN result = _ReadLong(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index) const { LOCK_BEGIN _WriteLong(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteLong(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index) const { LOCK_BEGIN _InsertLong(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertLong(objectProperty, value, key); LOCK_END }

    // size_t
    size_t BasePropertyAccessor::ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { size_t result = 0; LOCK_BEGIN result = _ReadUnsignedLong(objectProperty, index); LOCK_END return result; }
    size_t BasePropertyAccessor::ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { size_t result = 0; LOCK_BEGIN result = _ReadUnsignedLong(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index) const { LOCK_BEGIN _WriteUnsignedLong(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteUnsignedLong(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index) const { LOCK_BEGIN _InsertUnsignedLong(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertUnsignedLong(objectProperty, value, key); LOCK_END }

    // float
    float BasePropertyAccessor::ReadFloat(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { float result = 0; LOCK_BEGIN result = _ReadFloat(objectProperty, index); LOCK_END return result; }
    float BasePropertyAccessor::ReadFloat(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { float result = 0; LOCK_BEGIN result = _ReadFloat(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index) const { LOCK_BEGIN _WriteFloat(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteFloat(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index) const { LOCK_BEGIN _InsertFloat(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertFloat(objectProperty, value, key); LOCK_END }

    // double
    double BasePropertyAccessor::ReadDouble(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { double result = 0; LOCK_BEGIN result = _ReadDouble(objectProperty, index); LOCK_END return result; }
    double BasePropertyAccessor::ReadDouble(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { double result = 0; LOCK_BEGIN result = _ReadDouble(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index) const { LOCK_BEGIN _WriteDouble(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteDouble(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index) const { LOCK_BEGIN _InsertDouble(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertDouble(objectProperty, value, key); LOCK_END }

    // string
    std::string BasePropertyAccessor::ReadString(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { std::string result = ""; LOCK_BEGIN result = _ReadString(objectProperty, index); LOCK_END return result; }
    std::string BasePropertyAccessor::ReadString(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { std::string result = ""; LOCK_BEGIN std::string result = _ReadString(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index) const { LOCK_BEGIN _WriteString(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteString(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index) const { LOCK_BEGIN _InsertString(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertString(objectProperty, value, key); LOCK_END }

    // wstring
    std::wstring BasePropertyAccessor::ReadWstring(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { std::wstring result = L""; LOCK_BEGIN result = _ReadWstring(objectProperty, index); LOCK_END return result; }
    std::wstring BasePropertyAccessor::ReadWstring(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { std::wstring result = L""; LOCK_BEGIN result = _ReadWstring(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index) const { LOCK_BEGIN _WriteWstring(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const { LOCK_BEGIN _WriteWstring(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index) const { LOCK_BEGIN _InsertWstring(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const { LOCK_BEGIN _InsertWstring(objectProperty, value, key); LOCK_END }

    // object
    std::shared_ptr<IObject> BasePropertyAccessor::ReadObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { std::shared_ptr<IObject> result = nullptr; LOCK_BEGIN result = _ReadObject(objectProperty, index); LOCK_END return result; }
    std::shared_ptr<IObject> BasePropertyAccessor::ReadObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { std::shared_ptr<IObject> result = nullptr; LOCK_BEGIN result = _ReadObject(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const { LOCK_BEGIN _WriteObject(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const { LOCK_BEGIN _WriteObject(objectProperty, value, key); LOCK_END }
    void BasePropertyAccessor::InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const { LOCK_BEGIN _InsertObject(objectProperty, value, index); LOCK_END }
    void BasePropertyAccessor::InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const { LOCK_BEGIN _InsertObject(objectProperty, value, key); LOCK_END }
    std::shared_ptr<IObject> BasePropertyAccessor::CloneObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index) const { std::shared_ptr<IObject> result = nullptr; LOCK_BEGIN result = _CloneObject(objectProperty, index); LOCK_END return result; }
    std::shared_ptr<IObject> BasePropertyAccessor::CloneObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const { std::shared_ptr<IObject> result = nullptr; LOCK_BEGIN result = _CloneObject(objectProperty, key); LOCK_END return result; }
    void BasePropertyAccessor::ClearObject(const LockType &lockType, const size_t &objectProperty) const { LOCK_BEGIN _ClearObject(objectProperty); LOCK_END }

    // vector, set, map
    size_t BasePropertyAccessor::GetContainerCount(const LockType &lockType, const size_t &objectProperty) const { size_t result = 0; LOCK_BEGIN result = _GetContainerCount(objectProperty); LOCK_END return result; }
    void BasePropertyAccessor::RemoveContainerElement(const LockType &lockType, const size_t &objectProperty) const { LOCK_BEGIN _RemoveContainerElement(objectProperty); LOCK_END }
    void BasePropertyAccessor::ClearContainer(const LockType &lockType, const size_t &objectProperty) const { LOCK_BEGIN _ClearContainer(objectProperty); LOCK_END }
}
