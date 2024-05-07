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
        TRY_CATCH(){
            result = GetJsonInternalNameValuePairs(key)->GetJsonInternalType();
        }
        return result;
    }

    std::vector<std::wstring> Json::GetKeys(/*bool isRecursive*/) const
    {
        std::vector<std::wstring> result;
        TRY_CATCH(){
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
        }
        return result;
    }

    bool Json::IsContainKey(const std::wstring &key/*, bool isRecursive*/) const
    {
        TRY_CATCH(){
            auto keys = GetKeys();//GetKeys(isRecursive);
            return std::find(keys.begin(), keys.end(), key) != keys.end();
        }
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
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairs(key)->GetJsonInternalType() == JsonInternalType::Null;
        }
        return false;        
    }
    
    void Json::SetNull(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalType(JsonInternalType::Null);
        }
    }
    
    void Json::AddNull(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    bool Json::GetBool(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairs(key)->GetJsonInternalValue() == L"true";
        }
        return false;
    }
    
    void Json::SetBool(const std::wstring &key, bool value) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalType(JsonInternalType::Boolean);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalValue(value ? L"true" : L"false");
        }
    }

    void Json::AddBool(const std::wstring &key, bool value) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    double Json::GetDouble(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairs(key)->GetJsonInternalValue());
        }
        return 0.0;        
    }

    size_t Json::GetDecimalPlaces(const std::wstring &key) const
    {
        size_t decPt = 0;
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            std::wstring value = GetString(key);
            decPt = value.rfind(L".");
            if (decPt == std::wstring::npos)
                decPt = 0;
        }
        return decPt;
    }

    void Json::SetDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalType(JsonInternalType::Number);
            std::wostringstream oss;
            oss << std::fixed << std::setprecision(decimalPlaces) << value;
            GetJsonInternalNameValuePairs(key)->SetJsonInternalValue(oss.str());
        }
    }

    void Json::AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    int Json::GetInt32(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return std::stoi(GetJsonInternalNameValuePairs(key)->GetJsonInternalValue());
        }
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return std::stoll(GetJsonInternalNameValuePairs(key)->GetJsonInternalValue());
        }
        return 0;
    }

    void Json::SetInt(const std::wstring &key, int value) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalType(JsonInternalType::Number);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalValue(std::to_wstring(value));
        }
    }

    void Json::AddInt(const std::wstring &key, int value) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    std::wstring Json::GetString(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairs(key)->GetJsonInternalValue();
        }
        return L"";
    }
    
    void Json::SetString(const std::wstring &key, const std::wstring &value) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalType(JsonInternalType::String);
            GetJsonInternalNameValuePairs(key)->SetJsonInternalValue(value);
        }
    }

    void Json::AddString(const std::wstring &key, const std::wstring &value) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            if (GetJsonInternalNameValuePairs(key)->GetJsonInternalArray().size() > 0)
                return GetJsonInternalNameValuePairs(key)->GetJsonInternalArray().at(0);
        }
        return nullptr;
    }
    
    // void Json::SetObject(const std::wstring &key, std::shared_ptr<Json> object) const
    // {
    //     TRY_CATCH(){
    //         object->SetJsonInternalType(JsonInternalType::Json);
    //         GetJsonInternalNameValuePairs(key)->ClearArray();
    //         GetJsonInternalNameValuePairs(key)->GetJsonInternalArray().push_back(std::make_pair(key, object));
    //     )
    // }

    void Json::AddObject(const std::wstring &key, std::shared_ptr<Json> object) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            object->SetJsonInternalType(JsonInternalType::Json);

            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Object);
            json->InsertJsonInternalArray(object);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        }
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key) const
    {
        TRY_CATCH(){
            ValidateKeyIsFound(key);
            return GetJsonInternalNameValuePairs(key)->GetJsonInternalArray();
        }
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }
    
    // void Json::SetArray(const std::wstring &key, std::shared_ptr<Json> array) const
    // {
    //     TRY_CATCH(){
    //         array->SetJsonInternalType(JsonInternalType::Array);
    //         GetJsonInternalNameValuePairs(key)->ClearArray();
    //         GetJsonInternalNameValuePairs(key)->InsertJsonInternalArray();
    //     )
    // }

    void Json::AddArray(const std::wstring &key, std::shared_ptr<Json> array) const
    {
        TRY_CATCH(){
            ValidateKeyNotFound(key);
            array->SetJsonInternalType(JsonInternalType::Array);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, array));
        }
    }

    void Json::AddArrayNull() const
    {
        TRY_CATCH(){
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Null);
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayBool(bool value) const
    {
        TRY_CATCH(){
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Boolean);
            json->SetJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayDouble(double value, size_t decimalPlaces) const
    {
        TRY_CATCH(){
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayInt(int value) const
    {
        TRY_CATCH(){
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Number);
            json->SetJsonInternalValue(std::to_wstring(value));
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayString(const std::wstring &value) const
    {
        TRY_CATCH(){
            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::String);
            json->SetJsonInternalValue(value);
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayObject(std::shared_ptr<Json> object) const
    {
        TRY_CATCH(){
            object->SetJsonInternalType(JsonInternalType::Json);

            DECLARE_SPTR(Json, json);
            json->SetJsonInternalType(JsonInternalType::Object);
            json->InsertJsonInternalArray(object);
            _JsonInternalArray.push_back(json);
        }
    }

    void Json::AddArrayArray(std::shared_ptr<Json> array) const
    {
        TRY_CATCH(){
            array->SetJsonInternalType(JsonInternalType::Array);
            _JsonInternalArray.push_back(array);
        }
    }
}
