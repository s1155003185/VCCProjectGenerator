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
        GETSET(JsonType, Type, JsonType::Null);
        // Json
        ORDERED_MAP_SPTR_R(std::wstring, Json, NameValuePairs);
        // Json Object
        GETSET(std::wstring, Value, L"");
        VECTOR_SPTR(Json, Array);
        GETSET_SPTR_NULL(Json, Object);

        public:
            Json() : BaseDocument() {}
            virtual ~Json() {}
            
            virtual std::shared_ptr<IObject> Clone() const override;

            JsonType GetJsonType(const std::wstring &key);
            std::vector<std::wstring> GetKeys(/*bool isRecursive = false*/); // TODO: Recursive
            bool IsContainKey(const std::wstring &key/*, bool isRecursive = false*/); // TODO: Recursive

            bool IsNull(const std::wstring &key);
            bool GetBool(const std::wstring &key);
            double GetDouble(const std::wstring &key);
            size_t GetDecimalPlaces(const std::wstring &key);
            int GetInt32(const std::wstring &key);
            int64_t GetInt64(const std::wstring &key);
            std::wstring GetString(const std::wstring &key);

            std::vector<std::shared_ptr<Json>> &GetArray(const std::wstring &key);
            std::shared_ptr<Json> GetObject(const std::wstring &key);
    };
}
