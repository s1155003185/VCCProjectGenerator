#include "json.hpp"

#include <iomanip>
#include <memory>
#include <string>
#include <sstream>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

namespace vcc
{
    std::shared_ptr<IObject> Json::Clone() const
    {
        std::shared_ptr<Json> obj = std::make_shared<Json>(*this);
        obj->CloneJsonInternalNameValuePairs(this->GetJsonInternalNameValuePairs());
        obj->CloneJsonInternalArray(this->GetJsonInternalArray());
        return obj;
    }

    JsonInternalType Json::GetJsonType(const std::wstring &key) const
    {
        JsonInternalType result = JsonInternalType::Null;
        TRY
            result = GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalType();
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
            return GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalType() == JsonInternalType::Null;
        CATCH
        return false;        
    }
    
    void Json::SetNull(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalType(JsonInternalType::Null);
        CATCH
    }
    
    void Json::AddNull(const std::wstring &key) const
    {
        TRY
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    bool Json::GetBool(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue() == L"true";
        CATCH
        return false;
    }
    
    void Json::SetBool(const std::wstring &key, bool value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalType(JsonInternalType::Boolean);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalValue(value ? L"true" : L"false");
        CATCH
    }

    void Json::AddBool(const std::wstring &key, bool value) const
    {
        TRY
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    float Json::GetFloat(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue());
        CATCH
        return 0.0;        
    }

    double Json::GetDouble(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue());
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
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalType(JsonInternalType::Number);
            std::wostringstream oss;
            oss << std::fixed << std::setprecision(decimalPlaces) << value;
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalValue(oss.str());
        CATCH
    }

    void Json::AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    int Json::GetInt32(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stoi(GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue());
        CATCH
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return std::stoll(GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue());
        CATCH
        return 0;
    }

    void Json::SetInt(const std::wstring &key, int64_t value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalType(JsonInternalType::Number);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalValue(std::to_wstring(value));
        CATCH
    }

    void Json::AddInt(const std::wstring &key, int64_t value) const
    {
        TRY
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    char Json::GetChar(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            std::wstring result = GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue();
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
            std::wstring result = GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue();
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
            return GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalValue();
        CATCH
        return L"";
    }
    
    void Json::SetString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalType(JsonInternalType::String);
            GetJsonInternalNameValuePairsByKey(key)->SetJsonInternalValue(value);
        CATCH
    }

    void Json::AddString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            if (GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalArray().size() > 0)
                return GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalArray().at(0);
        CATCH
        return nullptr;
    }
    
    // void Json::SetObject(const std::wstring &key, std::shared_ptr<Json> object) const
    // {
    //     TRY
    //         object->SetJsonInternalType(JsonInternalType::Json);
    //         GetJsonInternalNameValuePairsByKey(key)->ClearArray();
    //         GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalArray().push_back(std::make_pair(key, object));
    //     CATCH
    // }

    void Json::AddObject(const std::wstring &key, std::shared_ptr<Json> object) const
    {
        TRY
            ValidateKeyNotFound(key);
            object->SetJsonInternalType(JsonInternalType::Json);

            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Object);
            json->InsertJsonInternalArray(object);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key) const
    {
        TRY
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairsByKey(key)->GetJsonInternalArray();
        CATCH
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }
    
    // void Json::SetArray(const std::wstring &key, std::shared_ptr<Json> array) const
    // {
    //     TRY
    //         array->SetJsonInternalType(JsonInternalType::Array);
    //         GetJsonInternalNameValuePairsByKey(key)->ClearArray();
    //         GetJsonInternalNameValuePairsByKey(key)->InsertJsonInternalArray();
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
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayBool(bool value) const
    {
        TRY
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayDouble(double value, size_t decimalPlaces) const
    {
        TRY
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayInt(int64_t value) const
    {
        TRY
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayString(const std::wstring &value) const
    {
        TRY
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayObject(std::shared_ptr<Json> object) const
    {
        TRY
            object->SetJsonInternalType(JsonInternalType::Json);

            DECLARE_SPTR(Json, json);
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
