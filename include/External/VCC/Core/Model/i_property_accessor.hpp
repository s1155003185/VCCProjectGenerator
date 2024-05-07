#pragma once

#include <memory>
#include <string>

namespace vcc
{
    class IObject;
    enum class ITypeUnion;
    enum class LockType;

    // All property must be size_t for dynamic_cast object property type
    // Only allow basic type as need to export to interface
    // Virtual Function not support template, cannot batch update Container with property accessor
    class IPropertyAccessor
    {
        protected:
            IPropertyAccessor() {}
            ~IPropertyAccessor() {}

        public:
            // lock
            virtual void ReadLock() const = 0;
            virtual void WriteLock() const = 0;
            virtual void ReadWriteLock() const = 0;
            virtual void Unlock() const = 0;

            // bool
            virtual bool ReadBool(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual bool ReadBool(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index = -1) const = 0;
            virtual void WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const = 0;
            virtual void InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index = -1) const = 0;
            virtual void InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const = 0;

            // char, byte, int8_t
            virtual char ReadChar(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual char ReadChar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index = -1) const = 0;
            virtual void WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const = 0;
            virtual void InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index = -1) const = 0;
            virtual void InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const = 0;

            // wchar_t
            virtual wchar_t ReadWchar(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual wchar_t ReadWchar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const = 0;
            virtual void WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const = 0;
            virtual void InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const = 0;
            virtual void InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const = 0;

            // short
            virtual short ReadShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual short ReadShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index = -1) const = 0;
            virtual void WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const = 0;
            virtual void InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index = -1) const = 0;
            virtual void InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const = 0;

            // uint16_t
            virtual uint16_t ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual uint16_t ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const = 0;
            virtual void WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const = 0;
            virtual void InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const = 0;
            virtual void InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const = 0;

            // int, int32_t
            virtual int ReadInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual int ReadInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index = -1) const = 0;
            virtual void WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const = 0;
            virtual void InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index = -1) const = 0;
            virtual void InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const = 0;

            // uint32_t
            virtual uint32_t ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual uint32_t ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const = 0;
            virtual void WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const = 0;
            virtual void InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const = 0;
            virtual void InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const = 0;

            // long, int64_t, time_t, enum
            virtual long ReadLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual long ReadLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index = -1) const = 0;
            virtual void WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const = 0;
            virtual void InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index = -1) const = 0;
            virtual void InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const = 0;

            // size_t
            virtual size_t ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual size_t ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const = 0;
            virtual void WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const = 0;
            virtual void InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const = 0;
            virtual void InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const = 0;

            // float
            virtual float ReadFloat(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual float ReadFloat(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index = -1) const = 0;
            virtual void WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const = 0;
            virtual void InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index = -1) const = 0;
            virtual void InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const = 0;

            // double
            virtual double ReadDouble(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual double ReadDouble(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index = -1) const = 0;
            virtual void WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const = 0;
            virtual void InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index = -1) const = 0;
            virtual void InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const = 0;

            // string
            virtual std::string ReadString(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual std::string ReadString(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const = 0;
            virtual void WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const = 0;
            virtual void InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const = 0;
            virtual void InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const = 0;

            // wstring
            virtual std::wstring ReadWstring(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual std::wstring ReadWstring(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const = 0;
            virtual void WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const = 0;
            virtual void InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const = 0;
            virtual void InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const = 0;

            // object
            virtual std::shared_ptr<IObject> ReadObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual std::shared_ptr<IObject> ReadObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const = 0;
            virtual void WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const = 0;
            virtual void InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const = 0;
            virtual void InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const = 0;
            virtual std::shared_ptr<IObject> CloneObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0;
            virtual std::shared_ptr<IObject> CloneObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;
            virtual void ClearObject(const LockType &lockType, const size_t &objectProperty) const = 0;

            // vector, set, map
            virtual size_t GetContainerCount(const LockType &lockType, const size_t &objectProperty) const = 0;
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty) const = 0;
            virtual void ClearContainer(const LockType &lockType, const size_t &objectProperty) const = 0;
    };
}
