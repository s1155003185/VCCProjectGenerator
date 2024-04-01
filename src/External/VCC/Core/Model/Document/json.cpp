#include "json.hpp"

#include <memory>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::shared_ptr<IObject> JsonObject::Clone() const
    {
        std::shared_ptr<JsonObject> obj = std::make_shared<JsonObject>(*this);
        obj->CloneNameValuePairs(this->GetNameValuePairs());
        obj->CloneArray(this->GetArray());
        obj->CloneObject(this->GetObject());
        return obj;
    }

    JsonType JsonObject::GetJsonType(const std::wstring &key)
    {
        JsonType result = JsonType::Null;
        TRY_CATCH(
            result = GetNameValuePairs(key)->GetType();
        )
        return result;
    }

    std::vector<std::wstring> JsonObject::GetKeys(/*bool isRecursive*/)
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

    bool JsonObject::IsContainKey(const std::wstring &key/*, bool isRecursive*/)
    {
        TRY_CATCH(
            auto keys = GetKeys();//GetKeys(isRecursive);
            return std::find(keys.begin(), keys.end(), key) != keys.end();
        )
        return false;
    }

    bool JsonObject::IsNull(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetType() == JsonType::Null;
        )
        return false;        
    }

    bool JsonObject::GetBool(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue() == L"true";
        )
        return false;
    }

    double JsonObject::GetDouble(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stod(GetNameValuePairs(key)->GetValue());
        )
        return 0.0;        
    }

    size_t JsonObject::GetDecimalPlaces(const wstring &key)
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

    int JsonObject::GetInt32(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stoi(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    int64_t JsonObject::GetInt64(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stoll(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    std::wstring JsonObject::GetString(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue();
        )
        return L"";
    }

    std::vector<std::shared_ptr<JsonObject>> &JsonObject::GetArray(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetArray();
        )
        static std::vector<std::shared_ptr<JsonObject>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }

    std::shared_ptr<JsonObject> JsonObject::GetObject(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetObject();
        )
        return nullptr;
    }
}
