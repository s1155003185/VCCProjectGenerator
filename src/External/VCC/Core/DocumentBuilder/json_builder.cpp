#include "json_builder.hpp"

#include <string>

#include "i_document.hpp"
#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

const std::wstring nullStr = L"null";
const std::wstring trueStr = L"true";
const std::wstring falseStr = L"false";

namespace vcc
{
    std::wstring JsonBuilder::Serialize(const IDocument *doc)
    {
        std::wstring result = L"";
        TRY_CATCH(
            JsonObject *jsonObj = dynamic_cast<JsonObject *>(const_cast<IDocument *>(doc));
            assert(jsonObj != nullptr);
            switch (jsonObj->GetType())
            {
            // Value
            case JsonType::Boolean:
            case JsonType::Number:
                result = jsonObj->GetValue();
                break;
            case JsonType::Null:
                result = L"null";
                break;
            case JsonType::String:
                result = GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, jsonObj->GetValue());
                break;
            // Object
            case JsonType::Array: {
                for (auto const &element : jsonObj->GetArray()) {
                    if (!result.empty())
                        result += L",";
                    result += Serialize(element.get());
                }
                result = L"[" + result + L"]";
                break;
            }
            case JsonType::Object:
                result = Serialize(jsonObj->GetObject().get());
                break;
            case JsonType::Json: {
                for (auto const &pair : jsonObj->GetNameValuePairs()) {
                    if (!result.empty())
                        result += L",";
                    result += GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, pair.first);
                    result += L":";
                    result += Serialize(pair.second.get());
                }
                result = L"{" + result + L"}";
                break;
            }
            default:
                assert(false);
                break;
            }
        )
        return result;
    }

    std::wstring JsonBuilder::GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
    {
        return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
    }

    void JsonBuilder::ParseJsonObject(const std::wstring &str, size_t &pos, std::shared_ptr<JsonObject> doc)
    {
        TRY_CATCH(
            GetNextCharPos(str, pos, true);
            if (HasPrefix(str, nullStr, pos)) {
                doc->SetType(JsonType::Null);
                pos += nullStr.length() - 1;
            } else if (HasPrefix(str, trueStr, pos)) {
                doc->SetType(JsonType::Boolean);
                doc->SetValue(trueStr);
                pos += trueStr.length() - 1;
            } else if (HasPrefix(str, falseStr, pos)) {
                doc->SetType(JsonType::Boolean);
                doc->SetValue(falseStr);
                pos += falseStr.length() - 1;
            } else if (str[pos] == L'"') {
                std::wstring value = GetNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                value  = GetUnescapeStringWithQuote(EscapeStringType::DoubleQuote, value);
                doc->SetType(JsonType::String);
                doc->SetValue(value);
            } else if (str[pos] == L'\'') {
                std::wstring value = GetNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                value  = GetUnescapeStringWithQuote(EscapeStringType::SingleQuote, value);
                doc->SetType(JsonType::String);
                doc->SetValue(value);
            } else if (str[pos] == L'{') {
                DECLARE_SPTR(JsonObject, jsonObj);
                Deserialize(str, pos, jsonObj);
                doc->SetType(JsonType::Object);
                doc->SetObject(jsonObj);
            } else if (str[pos] == L'['){
                doc->SetType(JsonType::Array);
                GetNextCharPos(str, pos, false);
                while (pos < str.length()) {
                    DECLARE_SPTR(JsonObject, obj);
                    ParseJsonObject(str, pos, obj);
                    doc->InsertArray(obj);
                    GetNextCharPos(str, pos, false);
                    if (str[pos] == L']')
                        break;
                    else if (str[pos] == L',')
                        GetNextCharPos(str, pos, false);
                    else
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(pos, str[pos], L"Array elements not end with , or ]"));
                }
            } else {
                size_t startPos = pos;
                std::wstring numStr = GetNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
                Trim(numStr);
                try
                {
                    std::stod(numStr);
                }
                catch(const std::exception& e)
                {
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(startPos, str[pos], L"Unknown json value format: " + numStr));
                }
                doc->SetType(JsonType::Number);
                doc->SetValue(numStr);
            }
        )
    }

    void JsonBuilder::Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc)
    {
        TRY_CATCH(
            std::shared_ptr<JsonObject> jsonObj = dynamic_pointer_cast<JsonObject>(doc);
            assert(jsonObj != nullptr);
            GetNextCharPos(str, pos, true);
            if (str[pos] != L'{')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(pos, str[pos], L"Json Object not start with {"));
            GetNextCharPos(str, pos, false);
            while (pos < str.length())
            {
                // name
                std::wstring name = GetNextQuotedString(str, pos, {L":"}, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"});
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

                // value
                DECLARE_SPTR(JsonObject, obj);
                ParseJsonObject(str, pos, obj);
                jsonObj->SetType(JsonType::Json);
                jsonObj->InsertNameValuePairs(name, obj);

                GetNextCharPos(str, pos, false);
                if (str[pos] != L',')
                    break;
                GetNextCharPos(str, pos, false);
            }
        )
    }

    void JsonBuilder::Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc)
    {
        TRY_CATCH(
            size_t pos = 0;
            this->Deserialize(str, pos, doc);
        )
    }
}
