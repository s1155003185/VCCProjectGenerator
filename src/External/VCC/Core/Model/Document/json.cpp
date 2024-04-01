#include "json.hpp"

#include <memory>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::shared_ptr<IObject> Json::Clone() const
    {
        std::shared_ptr<Json> obj = std::make_shared<Json>(*this);
        obj->CloneNameValuePairs(this->GetNameValuePairs());
        obj->CloneArray(this->GetArray());
        obj->CloneObject(this->GetObject());
        return obj;
    }

    JsonType Json::GetJsonType(const std::wstring &key)
    {
        JsonType result = JsonType::Null;
        TRY_CATCH(
            result = GetNameValuePairs(key)->GetType();
        )
        return result;
    }

    std::vector<std::wstring> Json::GetKeys(/*bool isRecursive*/)
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

    bool Json::IsContainKey(const std::wstring &key/*, bool isRecursive*/)
    {
        TRY_CATCH(
            auto keys = GetKeys();//GetKeys(isRecursive);
            return std::find(keys.begin(), keys.end(), key) != keys.end();
        )
        return false;
    }

    bool Json::IsNull(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetType() == JsonType::Null;
        )
        return false;        
    }

    bool Json::GetBool(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue() == L"true";
        )
        return false;
    }

    double Json::GetDouble(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stod(GetNameValuePairs(key)->GetValue());
        )
        return 0.0;        
    }

    size_t Json::GetDecimalPlaces(const std::wstring &key)
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

    int Json::GetInt32(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stoi(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key)
    {
        TRY_CATCH(
            return std::stoll(GetNameValuePairs(key)->GetValue());
        )
        return 0;
    }

    std::wstring Json::GetString(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetValue();
        )
        return L"";
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetArray();
        )
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key)
    {
        TRY_CATCH(
            return GetNameValuePairs(key)->GetObject();
        )
        return nullptr;
    }
}
