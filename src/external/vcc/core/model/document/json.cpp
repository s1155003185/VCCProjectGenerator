#include "json.hpp"

#include <iomanip>
#include <memory>
#include <string>
#include <sstream>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

namespace vcc
{
    std::shared_ptr<IObject> Json::Clone() const
    {
        auto obj = std::make_shared<Json>(*this);
        obj->CloneJsonInternalNameValuePairs(this->GetJsonInternalNameValuePairs());
        obj->CloneJsonInternalArray(this->GetJsonInternalArray());
        return obj;
    }

    JsonInternalType Json::GetJsonType(const std::wstring &key) const
    {
        JsonInternalType result = JsonInternalType::Null;
        TRY
            result = GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalType();
        CATCH
        return result;
    }

    std::vector<std::wstring> Json::GetKeys(/*bool isRecursive*/) const
    {
        std::vector<std::wstring> result;
        TRY
            for (auto const &pair : _JsonInternalNameValuePairs) {
                result.push_back(pair.first);
                // if (isRecursive) {
                //     if (pair.second->GetJsonInternalType() == JsonInternalType::Array) {
                //         for (size_t i = 0; i < pair.second->GetJsonInternalArray().size(); i++) {

                //         }
                //     } else if (pair.second->GetJsonInternalType() == JsonInternalType::Object) {
                //         for (auto const &str : pair.second->GetKeys(true))
                //             result.push_back(ConcatPaths({pair.first, str}));
                //     }
                // }
            }
        CATCH
        return result;
    }

    bool Json::IsContainKey(const std::wstring &key/*, bool isRecursive*/) const
    {
        TRY
            auto keys = GetKeys();//GetKeys(isRecursive);
            return IsContain(keys, key);
        CATCH
        return false;
    }
    
    void Json::ValidateKeyIsFound(const std::wstring &key) const
    {
        if (!IsContainKey(key))
            THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Key " + key + L" not found.");
    }

    void Json::ValidateKeyNotFound(const std::wstring &key) const
    {
        if (IsContainKey(key))
            THROW_EXCEPTION_MSG(ExceptionType::KeyDuplicated, L"Key " + key + L" duplicated.");
    }

    bool Json::IsNull(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalType() == JsonInternalType::Null;
        CATCH
        return false;        
    }
    
    void Json::SetNull(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalType(JsonInternalType::Null);
        CATCH
    }
    
    void Json::AddNull(const std::wstring &key) const
    {
        TRY
            ValidateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    bool Json::GetBool(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue() == L"true";
        CATCH
        return false;
    }
    
    void Json::SetBool(const std::wstring &key, bool value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalType(JsonInternalType::Boolean);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalValue(value ? L"true" : L"false");
        CATCH
    }

    void Json::AddBool(const std::wstring &key, bool value) const
    {
        TRY
            ValidateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    float Json::GetFloat(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue());
        CATCH
        return 0.0;        
    }

    double Json::GetDouble(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue());
        CATCH
        return 0.0;        
    }

    size_t Json::GetDecimalPlaces(const std::wstring &key) const
    {
        size_t decPt = 0;
        TRY
            ValidateKeyIsFound(key);
            std::wstring value = GetString(key);
            decPt = value.rfind(L".");
            if (decPt == std::wstring::npos)
                decPt = 0;
        CATCH
        return decPt;
    }

    void Json::SetDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalType(JsonInternalType::Number);
            std::wostringstream oss;
            oss << std::fixed << std::setprecision(decimalPlaces) << value;
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalValue(oss.str());
        CATCH
    }

    void Json::AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY
            ValidateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    int Json::GetInt32(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stoi(GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue());
        CATCH
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stoll(GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue());
        CATCH
        return 0;
    }

    void Json::SetInt(const std::wstring &key, int64_t value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalType(JsonInternalType::Number);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalValue(std::to_wstring(value));
        CATCH
    }

    void Json::AddInt(const std::wstring &key, int64_t value) const
    {
        TRY
            ValidateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    char Json::GetChar(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            std::wstring result = GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue();
            if (result.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value of key " + key + L" is not char");
            return wstr2str(result)[0];
        CATCH
        return L'\0';
    }

    wchar_t Json::GetWchar(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            std::wstring result = GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue();
            if (result.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value of key " + key + L" is not wchar_t");
            return result[0];
        CATCH
        return L'\0';
    }
    
    std::wstring Json::GetString(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalValue();
        CATCH
        return L"";
    }
    
    void Json::SetString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalType(JsonInternalType::String);
            GetJsonInternalNameValuePairsAtKey(key)->SetJsonInternalValue(value);
        CATCH
    }

    void Json::AddString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            ValidateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            if (GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalArray().size() > 0)
                return GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalArray().at(0);
        CATCH
        return nullptr;
    }
    
    // void Json::SetObject(const std::wstring &key, std::shared_ptr<Json> object) const
    // {
    //     TRY
    //         object->SetJsonInternalType(JsonInternalType::Json);
    //         GetJsonInternalNameValuePairsAtKey(key)->ClearArray();
    //         GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalArray().push_back(std::make_pair(key, object));
    //     CATCH
    // }

    void Json::AddObject(const std::wstring &key, std::shared_ptr<Json> object) const
    {
        TRY
            ValidateKeyNotFound(key);
            object->SetJsonInternalType(JsonInternalType::Json);

            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Object);
            json->InsertJsonInternalArray(object);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsAtKey(key)->GetJsonInternalArray();
        CATCH
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }
    
    // void Json::SetArray(const std::wstring &key, std::shared_ptr<Json> array) const
    // {
    //     TRY
    //         array->SetJsonInternalType(JsonInternalType::Array);
    //         GetJsonInternalNameValuePairsAtKey(key)->ClearArray();
    //         GetJsonInternalNameValuePairsAtKey(key)->InsertJsonInternalArray();
    //     CATCH
    // }

    void Json::AddArray(const std::wstring &key, std::shared_ptr<Json> array) const
    {
        TRY
            ValidateKeyNotFound(key);
            array->SetJsonInternalType(JsonInternalType::Array);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, array));
        CATCH
    }

    void Json::AddArrayNull() const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    bool Json::GetArrayElementBool() const
    {
        TRY
            return _JsonInternalValue == L"true" ? true : false; 
        CATCH
        return false;
    }

    void Json::AddArrayBool(bool value) const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalArray.push_back(json);
        CATCH
    }
    
    double Json::GetArrayElementDouble() const
    {
        TRY
            return std::stod(_JsonInternalValue); 
        CATCH
        return 0;
    }

    void Json::AddArrayDouble(double value, size_t decimalPlaces) const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalArray.push_back(json);
        CATCH
    }

    int64_t Json::GetArrayElementInt64() const
    {
        TRY
            return std::stoi(_JsonInternalValue); 
        CATCH
        return 0;
    }

    void Json::AddArrayInt(int64_t value) const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalArray.push_back(json);
        CATCH
    }

    char Json::GetArrayElementChar() const
    {
        TRY
            if (_JsonInternalValue.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value is not wchar_t");
            return _JsonInternalValue[0];
        CATCH
        return '\0';
    }

    wchar_t Json::GetArrayElementWchar() const
    {
        TRY
            if (_JsonInternalValue.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value is not wchar_t");
            return _JsonInternalValue[0];
        CATCH
        return L'\0';
    }

    std::wstring Json::GetArrayElementString() const
    {
        TRY
            return _JsonInternalValue;
        CATCH
        return L"";
    }

    void Json::AddArrayString(const std::wstring &value) const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    std::shared_ptr<Json> Json::GetArrayElementObject() const
    {
        TRY
            return _JsonInternalArray.empty() ? nullptr : _JsonInternalArray[0];
        CATCH
        return nullptr;
    }

    void Json::AddArrayObject(std::shared_ptr<Json> object) const
    {
        TRY
            object->SetJsonInternalType(JsonInternalType::Json);

            auto json = std::make_shared<Json>();
            json->SetJsonInternalType(JsonInternalType::Object);
            json->InsertJsonInternalArray(object);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayArray(std::shared_ptr<Json> array) const
    {
        TRY
            array->SetJsonInternalType(JsonInternalType::Array);
            _JsonInternalArray.push_back(array);
        CATCH
    }
}
