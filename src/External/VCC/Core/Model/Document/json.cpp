#include "json.hpp"

#include <memory>

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
        obj->CloneNameValuePairs(this->GetNameValuePairs());
        obj->CloneArray(this->GetArray());
        return obj;
    }

    JsonType Json::GetJsonType(const std::wstring &key) const
    {
        JsonType result = JsonType::Null;
        TRY_CATCH(
            result = GetNameValuePairs(key)->GetType();
        )
        return result;
    }

    std::vector<std::wstring> Json::GetKeys(/*bool isRecursive*/) const
    {
        std::vector<std::wstring> result;
        TRY_CATCH(
            for (auto const &pair : _NameValuePairs) {
                result.push_back(pair.first);
                // if (isRecursive) {
                //     if (pair.second->GetType() == JsonType::Array) {
                //         for (size_t i = 0; i < pair.second->GetArray().size(); i++) {

                //         }
                //     } else if (pair.second->GetType() == JsonType::Object) {
                //         for (auto const &str : pair.second->GetKeys(true))
                //             result.push_back(ConcatPaths({pair.first, str}));
                //     }
                // }
            }
        )
        return result;
    }

    bool Json::IsContainKey(const std::wstring &key/*, bool isRecursive*/) const
    {
        TRY_CATCH(
            auto keys = GetKeys();//GetKeys(isRecursive);
            return std::find(keys.begin(), keys.end(), key) != keys.end();
        )
        return false;
    }

    bool Json::IsNull(const std::wstring &key) const
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetType() == JsonType::Null;
        )
        return false;        
    }
    
    void Json::AddNull(const std::wstring &key) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Null);
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    bool Json::GetBool(const std::wstring &key) const
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue() == L"true";
        )
        return false;
    }

    void Json::AddBool(const std::wstring &key, bool value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Boolean);
            json->SetValue(value ? L"true" : L"false");
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    double Json::GetDouble(const std::wstring &key) const
    {
        TRY_CATCH(
            return std::stod(GetNameValuePairs(key)->GetValue());
        )
        return 0.0;        
    }

    size_t Json::GetDecimalPlaces(const std::wstring &key) const
    {
        size_t decPt = 0;
        TRY_CATCH(
            std::wstring value = GetString(key);
            decPt = value.rfind(L".");
            if (decPt == std::wstring::npos)
                decPt = 0;
        )
        return decPt;
    }

    void Json::AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Number);
            json->SetValue(ToString(value, decimalPlaces));
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    int Json::GetInt32(const std::wstring &key) const
    {
        TRY_CATCH(
            return std::stoi(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key) const
    {
        TRY_CATCH(
            return std::stoll(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    void Json::AddInt(const std::wstring &key, int value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Number);
            json->SetValue(std::to_wstring(value));
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    std::wstring Json::GetString(const std::wstring &key) const
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue();
        )
        return L"";
    }

    void Json::AddString(const std::wstring &key, const std::wstring &value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::String);
            json->SetValue(value);
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key) const
    {
        TRY_CATCH(
            if (GetNameValuePairs(key)->GetArray().size() > 0)
                return GetNameValuePairs(key)->GetArray().at(0);
        )
        return nullptr;
    }

    void Json::AddObject(const std::wstring &key, std::shared_ptr<Json> object) const
    {
        TRY_CATCH(
            object->SetType(JsonType::Json);

            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Object);
            json->InsertArray(object);
            _NameValuePairs.push_back(std::make_pair(key, json));
        )
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key) const
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetArray();
        )
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }

    void Json::AddArray(const std::wstring &key, std::shared_ptr<Json> array) const
    {
        TRY_CATCH(
            array->SetType(JsonType::Array);
            _NameValuePairs.push_back(std::make_pair(key, array));
        )
    }

    void Json::AddArrayNull() const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Null);
            _Array.push_back(json);
        )
    }

    void Json::AddArrayBool(bool value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Boolean);
            json->SetValue(value ? L"true" : L"false");
            _Array.push_back(json);
        )
    }

    void Json::AddArrayDouble(double value, size_t decimalPlaces) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Number);
            json->SetValue(ToString(value, decimalPlaces));
            _Array.push_back(json);
        )
    }

    void Json::AddArrayInt(int value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Number);
            json->SetValue(std::to_wstring(value));
            _Array.push_back(json);
        )
    }

    void Json::AddArrayString(const std::wstring &value) const
    {
        TRY_CATCH(
            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::String);
            json->SetValue(value);
            _Array.push_back(json);
        )
    }

    void Json::AddArrayObject(std::shared_ptr<Json> object) const
    {
        TRY_CATCH(
            object->SetType(JsonType::Json);

            DECLARE_SPTR(Json, json);
            json->SetType(JsonType::Object);
            json->InsertArray(object);
            _Array.push_back(json);
        )
    }

    void Json::AddArrayArray(std::shared_ptr<Json> array) const
    {
        TRY_CATCH(
            array->SetType(JsonType::Array);
            _Array.push_back(array);
        )
    }
}
