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

    class Json : public BaseDocument
    {
        GETSET(JsonInternalType, JsonInternalType, JsonInternalType::Json);
        // Json
        ORDERED_MAP_SPTR_R(std::wstring, Json, JsonInternalNameValuePairs);
        // Json Object
        GETSET(std::wstring, JsonInternalValue, L"");
        VECTOR_SPTR(Json, JsonInternalArray);

        private:
            void validateKeyIsFound(const std::wstring &key) const;
            void validateKeyNotFound(const std::wstring &key) const;

        public:
            Json() : BaseDocument() {}
            virtual ~Json() {}
            
            virtual std::shared_ptr<IObject> clone() const override;

            JsonInternalType getJsonType(const std::wstring &key) const;
            std::vector<std::wstring> getKeys(/*bool isRecursive = false*/) const; // TODO: Recursive
            bool isContainKey(const std::wstring &key/*, bool isRecursive = false*/) const; // TODO: Recursive

            bool isNull(const std::wstring &key) const;
            void setNull(const std::wstring &key) const;
            void addNull(const std::wstring &key) const;

            bool getBool(const std::wstring &key) const;
            void setBool(const std::wstring &key, bool value) const;
            void addBool(const std::wstring &key, bool value) const;

            float getFloat(const std::wstring &key) const;
            double getDouble(const std::wstring &key) const;
            size_t getDecimalPlaces(const std::wstring &key) const;
            void setDouble(const std::wstring &key, double value, size_t decimalPlaces) const;
            void addDouble(const std::wstring &key, double value, size_t decimalPlaces) const;

            int getInt32(const std::wstring &key) const;
            int64_t getInt64(const std::wstring &key) const;
            void setInt(const std::wstring &key, int64_t value) const;
            void addInt(const std::wstring &key, int64_t value) const;

            char getChar(const std::wstring &key) const;
            wchar_t getWchar(const std::wstring &key) const;
            std::wstring getString(const std::wstring &key) const;
            void setString(const std::wstring &key, const std::wstring &value) const;
            void addString(const std::wstring &key, const std::wstring &value) const;

            std::shared_ptr<Json> getObject(const std::wstring &key) const;
            //void setObject(const std::wstring &key, std::shared_ptr<Json> object) const;
            void addObject(const std::wstring &key, std::shared_ptr<Json> object) const;

            std::vector<std::shared_ptr<Json>> &getArray(const std::wstring &key) const;
            //void setArray(const std::wstring &key, std::shared_ptr<Json> array) const;
            void addArray(const std::wstring &key, std::shared_ptr<Json> array) const;
            
            // Array
            void addArrayNull() const;

            bool getArrayElementBool() const;
            void addArrayBool(bool value) const;

            double getArrayElementDouble() const;
            void addArrayDouble(double value, size_t decimalPlaces) const;
            
            int64_t getArrayElementInt64() const;
            void addArrayInt(int64_t value) const;

            char getArrayElementChar() const;
            wchar_t getArrayElementWchar() const;
            std::wstring getArrayElementString() const;
            void addArrayString(const std::wstring &value) const;

            std::shared_ptr<Json> getArrayElementObject() const;
            void addArrayObject(std::shared_ptr<Json> object) const;
            
            void addArrayArray(std::shared_ptr<Json> array) const;
    };
}
