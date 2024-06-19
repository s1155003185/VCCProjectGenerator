#pragma once

#include <memory>

#include "base_document.hpp"
#include "class_macro.hpp"

namespace vcc
{
    enum class JsonInternalType
    {
        Array,
        Boolean,
        Number,
        Null,
        Object,
        Json,
        String
    };

    class Json : public BaseDocument<Json>
    {
        GETSET(JsonInternalType, JsonInternalType, JsonInternalType::Json);
        // Json
        ORDERED_MAP_SPTR_R(std::wstring, Json, JsonInternalNameValuePairs);
        // Json Object
        GETSET(std::wstring, JsonInternalValue, L"");
        VECTOR_SPTR(Json, JsonInternalArray);

        private:
            void ValidateKeyIsFound(const std::wstring &key) const;
            void ValidateKeyNotFound(const std::wstring &key) const;

        public:
            Json() : BaseDocument() {}
            virtual ~Json() {}
            
            virtual std::shared_ptr<IObject> Clone() const override;

            JsonInternalType GetJsonType(const std::wstring &key) const;
            std::vector<std::wstring> GetKeys(/*bool isRecursive = false*/) const; // TODO: Recursive
            bool IsContainKey(const std::wstring &key/*, bool isRecursive = false*/) const; // TODO: Recursive

            bool IsNull(const std::wstring &key) const;
            void SetNull(const std::wstring &key) const;
            void AddNull(const std::wstring &key) const;

            bool GetBool(const std::wstring &key) const;
            void SetBool(const std::wstring &key, bool value) const;
            void AddBool(const std::wstring &key, bool value) const;

            float GetFloat(const std::wstring &key) const;
            double GetDouble(const std::wstring &key) const;
            size_t GetDecimalPlaces(const std::wstring &key) const;
            void SetDouble(const std::wstring &key, double value, size_t decimalPlaces) const;
            void AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const;

            int GetInt32(const std::wstring &key) const;
            int64_t GetInt64(const std::wstring &key) const;
            void SetInt(const std::wstring &key, int64_t value) const;
            void AddInt(const std::wstring &key, int64_t value) const;

            char GetChar(const std::wstring &key) const;
            wchar_t GetWchar(const std::wstring &key) const;
            std::wstring GetString(const std::wstring &key) const;
            void SetString(const std::wstring &key, const std::wstring &value) const;
            void AddString(const std::wstring &key, const std::wstring &value) const;

            std::shared_ptr<Json> GetObject(const std::wstring &key) const;
            //void SetObject(const std::wstring &key, std::shared_ptr<Json> object) const;
            void AddObject(const std::wstring &key, std::shared_ptr<Json> object) const;

            std::vector<std::shared_ptr<Json>> &GetArray(const std::wstring &key) const;
            //void SetArray(const std::wstring &key, std::shared_ptr<Json> array) const;
            void AddArray(const std::wstring &key, std::shared_ptr<Json> array) const;
            
            // Array
            void AddArrayNull() const;
            void AddArrayBool(bool value) const;
            void AddArrayDouble(double value, size_t decimalPlaces) const;
            void AddArrayInt(int64_t value) const;
            void AddArrayString(const std::wstring &value) const;
            void AddArrayObject(std::shared_ptr<Json> object) const;
            void AddArrayArray(std::shared_ptr<Json> array) const;
    };
}
