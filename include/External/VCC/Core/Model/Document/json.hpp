#pragma once

#include <memory>

#include "base_document.hpp"
#include "class_macro.hpp"

namespace vcc
{
    enum class JsonType
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
        GETSET(JsonType, Type, JsonType::Json);
        // Json
        ORDERED_MAP_SPTR_R(std::wstring, Json, NameValuePairs);
        // Json Object
        GETSET(std::wstring, Value, L"");
        VECTOR_SPTR(Json, Array);

        public:
            Json() : BaseDocument() {}
            virtual ~Json() {}
            
            virtual std::shared_ptr<IObject> Clone() const override;

            JsonType GetJsonType(const std::wstring &key);
            std::vector<std::wstring> GetKeys(/*bool isRecursive = false*/); // TODO: Recursive
            bool IsContainKey(const std::wstring &key/*, bool isRecursive = false*/); // TODO: Recursive

            bool IsNull(const std::wstring &key);
            void AddNull(const std::wstring &key);

            bool GetBool(const std::wstring &key);
            void AddBool(const std::wstring &key, bool value);

            double GetDouble(const std::wstring &key);
            size_t GetDecimalPlaces(const std::wstring &key);
            void AddDouble(const std::wstring &key, double value, size_t decimalPlaces);

            int GetInt32(const std::wstring &key);
            int64_t GetInt64(const std::wstring &key);
            void AddInt(const std::wstring &key, int value);

            std::wstring GetString(const std::wstring &key);
            void AddString(const std::wstring &key, const std::wstring &value);

            std::shared_ptr<Json> GetObject(const std::wstring &key);
            void AddObject(const std::wstring &key, std::shared_ptr<Json> object);

            std::vector<std::shared_ptr<Json>> &GetArray(const std::wstring &key);
            void AddArray(const std::wstring &key, std::shared_ptr<Json> array);
            
            // Array
            void AddArrayNull();
            void AddArrayBool(bool value);
            void AddArrayDouble(double value, size_t decimalPlaces);
            void AddArrayInt(int value);
            void AddArrayString(const std::wstring &value);
            void AddArrayObject(std::shared_ptr<Json> object);
            void AddArrayArray(std::shared_ptr<Json> array);
    };
}
