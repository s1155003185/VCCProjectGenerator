#include "json.hpp"

#include <iomanip>
#include <memory>
#include <string>
#include <sstream>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "number_helper.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

namespace vcc
{
    std::shared_ptr<IObject> Json::clone() const
    {
        auto obj = std::make_shared<Json>(*this);
        obj->cloneJsonInternalNameValuePairs(this->getJsonInternalNameValuePairs());
        obj->cloneJsonInternalArray(this->getJsonInternalArray());
        return obj;
    }

    JsonInternalType Json::GetJsonType(const std::wstring &key) const
    {
        JsonInternalType result = JsonInternalType::Null;
        TRY
            result = getJsonInternalNameValuePairsAtKey(key)->getJsonInternalType();
        CATCH
        return result;
    }

    std::vector<std::wstring> Json::getKeys(/*bool isRecursive*/) const
    {
        std::vector<std::wstring> result;
        TRY
            for (auto const &pair : _JsonInternalNameValuePairs) {
                result.push_back(pair.first);
                // if (isRecursive) {
                //     if (pair.second->getJsonInternalType() == JsonInternalType::Array) {
                //         for (size_t i = 0; i < pair.second->getJsonInternalArray().size(); i++) {

                //         }
                //     } else if (pair.second->getJsonInternalType() == JsonInternalType::Object) {
                //         for (auto const &str : pair.second->getKeys(true))
                //             result.push_back(concatPaths({pair.first, str}));
                //     }
                // }
            }
        CATCH
        return result;
    }

    bool Json::IsContainKey(const std::wstring &key/*, bool isRecursive*/) const
    {
        TRY
            auto keys = getKeys();//getKeys(isRecursive);
            return isContain(keys, key);
        CATCH
        return false;
    }
    
    void Json::validateKeyIsFound(const std::wstring &key) const
    {
        if (!IsContainKey(key))
            THROW_EXCEPTION_MSG(ExceptionType::KeyNotFound, L"Key " + key + L" not found.");
    }

    void Json::validateKeyNotFound(const std::wstring &key) const
    {
        if (IsContainKey(key))
            THROW_EXCEPTION_MSG(ExceptionType::KeyDuplicated, L"Key " + key + L" duplicated.");
    }

    bool Json::IsNull(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return getJsonInternalNameValuePairsAtKey(key)->getJsonInternalType() == JsonInternalType::Null;
        CATCH
        return false;        
    }
    
    void Json::SetNull(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalType(JsonInternalType::Null);
        CATCH
    }
    
    void Json::AddNull(const std::wstring &key) const
    {
        TRY
            validateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Null);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    bool Json::GetBool(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return getJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue() == L"true";
        CATCH
        return false;
    }
    
    void Json::SetBool(const std::wstring &key, bool value) const
    {
        TRY
            validateKeyIsFound(key);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalType(JsonInternalType::Boolean);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalValue(value ? L"true" : L"false");
        CATCH
    }

    void Json::AddBool(const std::wstring &key, bool value) const
    {
        TRY
            validateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Boolean);
            json->setJsonInternalValue(value ? L"true" : L"false");
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    float Json::GetFloat(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue());
        CATCH
        return 0.0;        
    }

    double Json::GetDouble(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return std::stod(GetJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue());
        CATCH
        return 0.0;        
    }

    size_t Json::GetDecimalPlaces(const std::wstring &key) const
    {
        size_t decPt = 0;
        TRY
            validateKeyIsFound(key);
            std::wstring value = getString(key);
            decPt = value.rfind(L".");
            if (decPt == std::wstring::npos)
                decPt = 0;
        CATCH
        return decPt;
    }

    void Json::SetDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY
            validateKeyIsFound(key);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalType(JsonInternalType::Number);
            std::wostringstream oss;
            oss << std::fixed << std::setprecision(decimalPlaces) << value;
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalValue(oss.str());
        CATCH
    }

    void Json::AddDouble(const std::wstring &key, double value, size_t decimalPlaces) const
    {
        TRY
            validateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Number);
            json->setJsonInternalValue(ToString(value, decimalPlaces));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    int Json::GetInt32(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return std::stoi(GetJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue());
        CATCH
        return 0;
    }

    int64_t Json::GetInt64(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return std::stoll(GetJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue());
        CATCH
        return 0;
    }

    void Json::SetInt(const std::wstring &key, int64_t value) const
    {
        TRY
            validateKeyIsFound(key);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalType(JsonInternalType::Number);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalValue(std::to_wstring(value));
        CATCH
    }

    void Json::AddInt(const std::wstring &key, int64_t value) const
    {
        TRY
            validateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Number);
            json->setJsonInternalValue(std::to_wstring(value));
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    char Json::GetChar(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            std::wstring result = getJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue();
            if (result.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value of key " + key + L" is not char");
            return wstr2str(result)[0];
        CATCH
        return L'\0';
    }

    wchar_t Json::GetWchar(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            std::wstring result = getJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue();
            if (result.length() != 1)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Value of key " + key + L" is not wchar_t");
            return result[0];
        CATCH
        return L'\0';
    }
    
    std::wstring Json::GetString(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return getJsonInternalNameValuePairsAtKey(key)->getJsonInternalValue();
        CATCH
        return L"";
    }
    
    void Json::SetString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            validateKeyIsFound(key);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalType(JsonInternalType::String);
            getJsonInternalNameValuePairsAtKey(key)->setJsonInternalValue(value);
        CATCH
    }

    void Json::AddString(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            validateKeyNotFound(key);
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::String);
            json->setJsonInternalValue(value);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::shared_ptr<Json> Json::GetObject(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            if (GetJsonInternalNameValuePairsAtKey(key)->getJsonInternalArray().size() > 0)
                return getJsonInternalNameValuePairsAtKey(key)->getJsonInternalArray().at(0);
        CATCH
        return nullptr;
    }
    
    // void Json::SetObject(const std::wstring &key, std::shared_ptr<Json> object) const
    // {
    //     TRY
    //         object->setJsonInternalType(JsonInternalType::Json);
    //         getJsonInternalNameValuePairsAtKey(key)->clearArray();
    //         getJsonInternalNameValuePairsAtKey(key)->getJsonInternalArray().push_back(std::make_pair(key, object));
    //     CATCH
    // }

    void Json::AddObject(const std::wstring &key, std::shared_ptr<Json> object) const
    {
        TRY
            validateKeyNotFound(key);
            object->setJsonInternalType(JsonInternalType::Json);

            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Object);
            json->insertJsonInternalArray(object);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, json));
        CATCH
    }

    std::vector<std::shared_ptr<Json>> &Json::GetArray(const std::wstring &key) const
    {
        TRY
            validateKeyIsFound(key);
            return getJsonInternalNameValuePairsAtKey(key)->getJsonInternalArray();
        CATCH
        static std::vector<std::shared_ptr<Json>> emptyVector;
        emptyVector.clear();
        return emptyVector;
    }
    
    // void Json::SetArray(const std::wstring &key, std::shared_ptr<Json> array) const
    // {
    //     TRY
    //         array->setJsonInternalType(JsonInternalType::Array);
    //         getJsonInternalNameValuePairsAtKey(key)->clearArray();
    //         getJsonInternalNameValuePairsAtKey(key)->insertJsonInternalArray();
    //     CATCH
    // }

    void Json::AddArray(const std::wstring &key, std::shared_ptr<Json> array) const
    {
        TRY
            validateKeyNotFound(key);
            array->setJsonInternalType(JsonInternalType::Array);
            _JsonInternalNameValuePairs.push_back(std::make_pair(key, array));
        CATCH
    }

    void Json::AddArrayNull() const
    {
        TRY
            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Null);
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
            json->setJsonInternalType(JsonInternalType::Boolean);
            json->setJsonInternalValue(value ? L"true" : L"false");
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
            json->setJsonInternalType(JsonInternalType::Number);
            json->setJsonInternalValue(ToString(value, decimalPlaces));
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
            json->setJsonInternalType(JsonInternalType::Number);
            json->setJsonInternalValue(std::to_wstring(value));
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
            json->setJsonInternalType(JsonInternalType::String);
            json->setJsonInternalValue(value);
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
            object->setJsonInternalType(JsonInternalType::Json);

            auto json = std::make_shared<Json>();
            json->setJsonInternalType(JsonInternalType::Object);
            json->insertJsonInternalArray(object);
            _JsonInternalArray.push_back(json);
        CATCH
    }

    void Json::AddArrayArray(std::shared_ptr<Json> array) const
    {
        TRY
            array->setJsonInternalType(JsonInternalType::Array);
            _JsonInternalArray.push_back(array);
        CATCH
    }
}
