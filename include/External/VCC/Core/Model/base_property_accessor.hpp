#pragma once
#include "i_property_accessor.hpp"

#include <string>
#include <shared_mutex>

#include "class_macro.hpp"
#include "i_object.hpp"

namespace vcc
{
    enum class LockType;
    class BasePropertyAccessor : public IPropertyAccessor
    {
        inline static std::shared_mutex _Mutex;
        GETSET_SPTR_NULL(IObject, Object);

        protected:
            BasePropertyAccessor(std::shared_ptr<IObject> object) { this->_Object = object; }
            ~BasePropertyAccessor() {}

            virtual bool _ReadBool(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual bool _ReadBool(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteBool(const size_t &objectProperty, const bool &value, const int64_t &index = -1) const;
            virtual void _WriteBool(const size_t &objectProperty, const bool &value, const ITypeUnion *key) const;
            virtual void _InsertBool(const size_t &objectProperty, const bool &value, const int64_t &index = -1) const;
            virtual void _InsertBool(const size_t &objectProperty, const bool &value, const ITypeUnion *key) const;

            // char, byte, int8_t
            virtual char _ReadChar(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual char _ReadChar(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteChar(const size_t &objectProperty, const char &value, const int64_t &index = -1) const;
            virtual void _WriteChar(const size_t &objectProperty, const char &value, const ITypeUnion *key) const;
            virtual void _InsertChar(const size_t &objectProperty, const char &value, const int64_t &index = -1) const;
            virtual void _InsertChar(const size_t &objectProperty, const char &value, const ITypeUnion *key) const;

            // wchar_t
            virtual wchar_t _ReadWchar(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual wchar_t _ReadWchar(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteWchar(const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const;
            virtual void _WriteWchar(const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const;
            virtual void _InsertWchar(const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const;
            virtual void _InsertWchar(const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const;

            // short
            virtual short _ReadShort(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual short _ReadShort(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteShort(const size_t &objectProperty, const short &value, const int64_t &index = -1) const;
            virtual void _WriteShort(const size_t &objectProperty, const short &value, const ITypeUnion *key) const;
            virtual void _InsertShort(const size_t &objectProperty, const short &value, const int64_t &index = -1) const;
            virtual void _InsertShort(const size_t &objectProperty, const short &value, const ITypeUnion *key) const;

            // uint16_t
            virtual uint16_t _ReadUnsignedShort(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual uint16_t _ReadUnsignedShort(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteUnsignedShort(const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const;
            virtual void _WriteUnsignedShort(const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const;
            virtual void _InsertUnsignedShort(const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const;
            virtual void _InsertUnsignedShort(const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const;

            // int, int32_t
            virtual int _ReadInt(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual int _ReadInt(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteInt(const size_t &objectProperty, const int &value, const int64_t &index = -1) const;
            virtual void _WriteInt(const size_t &objectProperty, const int &value, const ITypeUnion *key) const;
            virtual void _InsertInt(const size_t &objectProperty, const int &value, const int64_t &index = -1) const;
            virtual void _InsertInt(const size_t &objectProperty, const int &value, const ITypeUnion *key) const;

            // uint32_t
            virtual uint32_t _ReadUnsignedInt(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual uint32_t _ReadUnsignedInt(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteUnsignedInt(const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const;
            virtual void _WriteUnsignedInt(const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const;
            virtual void _InsertUnsignedInt(const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const;
            virtual void _InsertUnsignedInt(const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const;

            // long, int64_t, time_t, enum
            virtual long _ReadLong(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual long _ReadLong(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteLong(const size_t &objectProperty, const long &value, const int64_t &index = -1) const;
            virtual void _WriteLong(const size_t &objectProperty, const long &value, const ITypeUnion *key) const;
            virtual void _InsertLong(const size_t &objectProperty, const long &value, const int64_t &index = -1) const;
            virtual void _InsertLong(const size_t &objectProperty, const long &value, const ITypeUnion *key) const;

            // size_t
            virtual size_t _ReadUnsignedLong(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual size_t _ReadUnsignedLong(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteUnsignedLong(const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const;
            virtual void _WriteUnsignedLong(const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const;
            virtual void _InsertUnsignedLong(const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const;
            virtual void _InsertUnsignedLong(const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const;

            // float
            virtual float _ReadFloat(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual float _ReadFloat(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteFloat(const size_t &objectProperty, const float &value, const int64_t &index = -1) const;
            virtual void _WriteFloat(const size_t &objectProperty, const float &value, const ITypeUnion *key) const;
            virtual void _InsertFloat(const size_t &objectProperty, const float &value, const int64_t &index = -1) const;
            virtual void _InsertFloat(const size_t &objectProperty, const float &value, const ITypeUnion *key) const;

            // double
            virtual double _ReadDouble(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual double _ReadDouble(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteDouble(const size_t &objectProperty, const double &value, const int64_t &index = -1) const;
            virtual void _WriteDouble(const size_t &objectProperty, const double &value, const ITypeUnion *key) const;
            virtual void _InsertDouble(const size_t &objectProperty, const double &value, const int64_t &index = -1) const;
            virtual void _InsertDouble(const size_t &objectProperty, const double &value, const ITypeUnion *key) const;

            // string
            virtual std::string _ReadString(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual std::string _ReadString(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteString(const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const;
            virtual void _WriteString(const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const;
            virtual void _InsertString(const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const;
            virtual void _InsertString(const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const;

            // wstring
            virtual std::wstring _ReadWstring(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual std::wstring _ReadWstring(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteWstring(const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const;
            virtual void _WriteWstring(const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const;
            virtual void _InsertWstring(const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const;
            virtual void _InsertWstring(const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const;

            // object
            virtual std::shared_ptr<IObject> _ReadObject(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual std::shared_ptr<IObject> _ReadObject(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const;
            virtual void _WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const;
            virtual void _InsertObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const;
            virtual void _InsertObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const;
            virtual std::shared_ptr<IObject> _CloneObject(const size_t &objectProperty, const int64_t &index = -1) const;
            virtual std::shared_ptr<IObject> _CloneObject(const size_t &objectProperty, const ITypeUnion *key) const;
            virtual void _ClearObject(const size_t &objectProperty) const;

            // vector, set, map
            virtual size_t _GetContainerCount(const size_t &objectProperty) const;
            virtual void _RemoveContainerElement(const size_t &objectProperty) const;
            virtual void _ClearContainer(const size_t &objectProperty) const;

        public:
            // lock
            virtual void ReadLock() const override;
            virtual void WriteLock() const override;
            virtual void ReadWriteLock() const override;
            virtual void Unlock() const override;
            
            // bool
            virtual bool ReadBool(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual bool ReadBool(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index = -1) const override;
            virtual void WriteBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const override;
            virtual void InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const int64_t &index = -1) const override;
            virtual void InsertBool(const LockType &lockType, const size_t &objectProperty, const bool &value, const ITypeUnion *key) const override;

            // char, byte, int8_t
            virtual char ReadChar(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual char ReadChar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index = -1) const override;
            virtual void WriteChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const override;
            virtual void InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const int64_t &index = -1) const override;
            virtual void InsertChar(const LockType &lockType, const size_t &objectProperty, const char &value, const ITypeUnion *key) const override;

            // wchar_t
            virtual wchar_t ReadWchar(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual wchar_t ReadWchar(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const override;
            virtual void WriteWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const override;
            virtual void InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const int64_t &index = -1) const override;
            virtual void InsertWchar(const LockType &lockType, const size_t &objectProperty, const wchar_t &value, const ITypeUnion *key) const override;

            // short
            virtual short ReadShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual short ReadShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index = -1) const override;
            virtual void WriteShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const override;
            virtual void InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const int64_t &index = -1) const override;
            virtual void InsertShort(const LockType &lockType, const size_t &objectProperty, const short &value, const ITypeUnion *key) const override;

            // uint16_t
            virtual uint16_t ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual uint16_t ReadUnsignedShort(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const override;
            virtual void WriteUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const override;
            virtual void InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const int64_t &index = -1) const override;
            virtual void InsertUnsignedShort(const LockType &lockType, const size_t &objectProperty, const uint16_t &value, const ITypeUnion *key) const override;

            // int, int32_t
            virtual int ReadInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual int ReadInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index = -1) const override;
            virtual void WriteInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const override;
            virtual void InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const int64_t &index = -1) const override;
            virtual void InsertInt(const LockType &lockType, const size_t &objectProperty, const int &value, const ITypeUnion *key) const override;

            // uint32_t
            virtual uint32_t ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual uint32_t ReadUnsignedInt(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const override;
            virtual void WriteUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const override;
            virtual void InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const int64_t &index = -1) const override;
            virtual void InsertUnsignedInt(const LockType &lockType, const size_t &objectProperty, const uint32_t &value, const ITypeUnion *key) const override;

            // long, int64_t, time_t, enum
            virtual long ReadLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual long ReadLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index = -1) const override;
            virtual void WriteLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const override;
            virtual void InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const int64_t &index = -1) const override;
            virtual void InsertLong(const LockType &lockType, const size_t &objectProperty, const long &value, const ITypeUnion *key) const override;

            // size_t
            virtual size_t ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual size_t ReadUnsignedLong(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const override;
            virtual void WriteUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const override;
            virtual void InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const int64_t &index = -1) const override;
            virtual void InsertUnsignedLong(const LockType &lockType, const size_t &objectProperty, const size_t &value, const ITypeUnion *key) const override;

            // float
            virtual float ReadFloat(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual float ReadFloat(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index = -1) const override;
            virtual void WriteFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const override;
            virtual void InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const int64_t &index = -1) const override;
            virtual void InsertFloat(const LockType &lockType, const size_t &objectProperty, const float &value, const ITypeUnion *key) const override;

            // double
            virtual double ReadDouble(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual double ReadDouble(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index = -1) const override;
            virtual void WriteDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const override;
            virtual void InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const int64_t &index = -1) const override;
            virtual void InsertDouble(const LockType &lockType, const size_t &objectProperty, const double &value, const ITypeUnion *key) const override;

            // string
            virtual std::string ReadString(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual std::string ReadString(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const override;
            virtual void WriteString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const override;
            virtual void InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const int64_t &index = -1) const override;
            virtual void InsertString(const LockType &lockType, const size_t &objectProperty, const std::string &value, const ITypeUnion *key) const override;

            // wstring
            virtual std::wstring ReadWstring(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual std::wstring ReadWstring(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const override;
            virtual void WriteWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const override;
            virtual void InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const override;
            virtual void InsertWstring(const LockType &lockType, const size_t &objectProperty, const std::wstring &value, const ITypeUnion *key) const override;

            // object
            virtual std::shared_ptr<IObject> ReadObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual std::shared_ptr<IObject> ReadObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const override;
            virtual void WriteObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const override;
            virtual void InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const override;
            virtual void InsertObject(const LockType &lockType, const size_t &objectProperty, std::shared_ptr<IObject> value, const ITypeUnion *key) const override;
            virtual std::shared_ptr<IObject> CloneObject(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const override;
            virtual std::shared_ptr<IObject> CloneObject(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const override;
            virtual void ClearObject(const LockType &lockType, const size_t &objectProperty) const override;

            // vector, set, map
            virtual size_t GetContainerCount(const LockType &lockType, const size_t &objectProperty) const override;
            virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty) const override;
            virtual void ClearContainer(const LockType &lockType, const size_t &objectProperty) const override;
    };
}
