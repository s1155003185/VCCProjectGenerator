#include "json_builder.hpp"

#include <string>

#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring JsonBuilder::Serialize(const JsonObject *doc)
    {
        std::wstring result = L"{";
        //TRY_CATCH(
            if (doc->GetType() != JsonType::Json)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Json Object Type is not Json");
            std::wstring content = L"";
            for (auto const &pair : doc->GetNameValuePairs()) {
                if (!content.empty())
                    content += L",";
                content += GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, pair.first) + L":";
                auto const &obj = pair.second;
                switch (obj->GetType())
                {
                case JsonType::Null:
                    content += L"null";
                    break;
                case JsonType::Boolean:
                case JsonType::Number:
                    content += obj->GetValue();
                    break;
                case JsonType::String:
                    content += GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, obj->GetValue());
                    break;
                case JsonType::Array:
                    content += L"[  ]"; // TODO
                    break;
                case JsonType::Object:
                    content += Serialize(obj->GetObject().get());
                    break;    
                default:
                    assert(false);
                    break;
                }
            }
        //)
        result += content + L"}";
        return result;
    }

    std::wstring JsonBuilder::GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
    {
        return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
    }

    void JsonBuilder::ParseJsonObject(const std::wstring &str, std::shared_ptr<JsonObject> doc)
    {
        TRY_CATCH(
            if (str == L"null") {
                doc->SetType(JsonType::Null);
            } else if (str == L"true" || str == L"false") {
                doc->SetType(JsonType::Boolean);
                doc->SetValue(str);
            } else if (str.starts_with(L"'")) {
                std::wstring value = GetUnescapeStringWithQuote(EscapeStringType::SingleQuote, str);
                doc->SetType(JsonType::String);
                doc->SetValue(value);
            } else if (str.starts_with(L"\"")) {
                std::wstring value = GetUnescapeStringWithQuote(EscapeStringType::DoubleQuote, str);
                doc->SetType(JsonType::String);
                doc->SetValue(value);
            } else if (str.starts_with(L"{")) {
                DECLARE_SPTR(JsonObject, jsonObj);
                Deserialize(str, jsonObj);
                doc->SetType(JsonType::Object);
                doc->SetObject(jsonObj);
            } else {
                std::wstring numStr = str;
                Trim(numStr);
                try
                {
                    std::stod(str);
                }
                catch(const std::exception& e)
                {
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown json value format: " + str);
                }
                doc->SetType(JsonType::Number);
                doc->SetValue(numStr);
            }
        )
    }

    void JsonBuilder::Deserialize(const std::wstring &str, std::shared_ptr<JsonObject> doc)
    {
        //TRY_CATCH(
            size_t pos = 0;
            GetNextCharPos(str, pos, true);
            if (str[pos] != L'{')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(pos, str[pos], L"Json Object not start with {"));
            GetNextCharPos(str, pos, false);
            while (pos < str.length())
            {
                std::wstring name = GetNextString(str, pos, {L':'}, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                Trim(name);
                if (name.starts_with(L"\"")) {
                    name = GetUnescapeStringWithQuote(EscapeStringType::DoubleQuote, name);
                } else if (name.starts_with(L"\'")) {
                    name = GetUnescapeStringWithQuote(EscapeStringType::SingleQuote, name);
                }
                GetNextCharPos(str, pos, false);
                if (str[pos] != L':')
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(pos, str[pos], L"Json Object name " + name + L" not followed by :"));
                GetNextCharPos(str, pos, false);
                std::wstring value = GetNextString(str, pos, { L',', L'}' }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                Trim(value);
                if (value.ends_with(L",") || (!value.starts_with(L"{") && value.ends_with(L"}"))) {
                    value.pop_back();
                    pos--;
                }
                Trim(value);
                
                // special handling for array
                if (value.starts_with(L"[")) {
                    Trim(value);
                    value = value.substr(1, value.length() - 2);
                    size_t arrayPos = 0;
                    DECLARE_SPTR(JsonObject, arrayObj);
                    arrayObj->SetType(JsonType::Array);
                    while (arrayPos < value.length()) {
                        std::wstring objStr = GetNextString(value, arrayPos, { L',' }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                        
                        DECLARE_SPTR(JsonObject, obj);
                        Deserialize(objStr, obj);
                        arrayObj->InsertArray(obj);
                        GetNextCharPos(value, arrayPos, false);
                        if (value[arrayPos] != L',')
                            break;
                        GetNextCharPos(value, arrayPos, false);
                    }
                    doc->SetType(JsonType::Json);
                    doc->InsertNameValuePairs(name, arrayObj);
                } else {
                    DECLARE_SPTR(JsonObject, obj);
                    ParseJsonObject(value, obj);
                    doc->SetType(JsonType::Json);
                    doc->InsertNameValuePairs(name, obj);
                }

                GetNextCharPos(str, pos, false);
                if (str[pos] != L',')
                    break;
                GetNextCharPos(str, pos, false);
            }
        //)
    }
}
