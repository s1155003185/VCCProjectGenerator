#include "json_builder.hpp"

#include <string>

#include "i_document.hpp"
#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "json.hpp"
#include "string_helper.hpp"

const std::wstring nullStr = L"null";
const std::wstring trueStr = L"true";
const std::wstring falseStr = L"false";

namespace vcc
{    
    std::wstring JsonBuilder::getCurrentIndent() const
    {
        if (!_IsBeautify)
            return L"";
        std::wstring result = L"";
        for (int64_t i = 0; i < _Level; i++) {
            result += _Indent;
        }
        return result;
    }

    std::wstring JsonBuilder::serialize(const IDocument *doc) const
    {
        std::wstring result = L"";
        TRY
            std::wstring currentNewLineCharacter = _IsBeautify ? _NewLineCharacter : L"";
            std::wstring currentNameColonSpace = _IsBeautify ? _NameColonSpace : L"";
            std::wstring currentColonValueSpace = _IsBeautify ? _ColonValueSpace : L"";

            Json *jsonObj = dynamic_cast<Json *>(const_cast<IDocument *>(doc));
            assert(jsonObj != nullptr);
            switch (jsonObj->getJsonInternalType())
            {
            // Value
            case JsonInternalType::Boolean:
            case JsonInternalType::Number:
                result = jsonObj->getJsonInternalValue();
                break;
            case JsonInternalType::Null:
                result = L"null";
                break;
            case JsonInternalType::String:
                result = getEscapeStringWithQuote(EscapeStringType::DoubleQuote, jsonObj->getJsonInternalValue());
                break;
            // Object
            case JsonInternalType::Array: {
                _Level++;
                for (auto const &element : jsonObj->getJsonInternalArray()) {
                    if (!result.empty())
                        result += L"," + currentNewLineCharacter;
                    result += getCurrentIndent() + serialize(element.get());
                }
                _Level--;
                result = L"[" + currentNewLineCharacter + result + currentNewLineCharacter + getCurrentIndent() + L"]";
                break;
            }
            case JsonInternalType::Object:
                if (jsonObj->getJsonInternalArray().at(0).get() != nullptr)
                    result = serialize(jsonObj->getJsonInternalArray().at(0).get());
                else
                    result = L"null";
                break;
            case JsonInternalType::Json: {
                _Level++;
                for (auto const &pair : jsonObj->getJsonInternalNameValuePairs()) {
                    if (!result.empty())
                        result += L"," + currentNewLineCharacter;
                    result += getCurrentIndent() + getEscapeStringWithQuote(EscapeStringType::DoubleQuote, pair.first);
                    result += currentNameColonSpace + L":" + currentColonValueSpace;
                    result += serialize(pair.second.get());
                }
                _Level--;
                result = L"{" + currentNewLineCharacter + result + currentNewLineCharacter + getCurrentIndent() + L"}";
                break;
            }
            default:
                assert(false);
                break;
            }
        CATCH
        return result;
    }

    void JsonBuilder::ParseJsonObject(const std::wstring &str, size_t &pos, std::shared_ptr<Json> doc) const
    {
        size_t startPos = pos;
        try {
            getNextCharPos(str, pos, true);
            if (isStartWith(str, nullStr, pos)) {
                doc->setJsonInternalType(JsonInternalType::Null);
                pos += nullStr.length() - 1;
            } else if (isStartWith(str, trueStr, pos)) {
                doc->setJsonInternalType(JsonInternalType::Boolean);
                doc->setJsonInternalValue(trueStr);
                pos += trueStr.length() - 1;
            } else if (isStartWith(str, falseStr, pos)) {
                doc->setJsonInternalType(JsonInternalType::Boolean);
                doc->setJsonInternalValue(falseStr);
                pos += falseStr.length() - 1;
            } else if (str[pos] == L'"') {
                std::wstring value = getNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"}, { L"\"", L"'" });
                value  = getUnescapeStringWithQuote(EscapeStringType::DoubleQuote, value);
                doc->setJsonInternalType(JsonInternalType::String);
                doc->setJsonInternalValue(value);
            } else if (str[pos] == L'\'') {
                std::wstring value = getNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"}, { L"\"", L"'" });
                value  = getUnescapeStringWithQuote(EscapeStringType::SingleQuote, value);
                doc->setJsonInternalType(JsonInternalType::String);
                doc->setJsonInternalValue(value);
            } else if (str[pos] == L'{') {
                auto jsonObj = std::make_shared<Json>();
                deserialize(str, pos, jsonObj);
                doc->setJsonInternalType(JsonInternalType::Object);
                doc->insertJsonInternalArray(jsonObj);
            } else if (str[pos] == L'['){
                doc->setJsonInternalType(JsonInternalType::Array);
                getNextCharPos(str, pos, false);
                if (str[pos] != L']') {
                    while (pos < str.length()) {
                        auto obj = std::make_shared<Json>();
                        ParseJsonObject(str, pos, obj);
                        doc->insertJsonInternalArray(obj);
                        getNextCharPos(str, pos, false);
                        if (str[pos] == L']')
                            break;
                        else if (str[pos] == L',')
                            getNextCharPos(str, pos, false);
                        else
                            THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(str, pos, L"Array elements not end with , or ]"));
                    }
                }
            } else {
                std::wstring numStr = getNextQuotedString(str, pos, { L",", L"}", L"]" }, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"}, { L"\"", L"'" });
                trim(numStr);
                try
                {
                    std::stod(numStr);
                }
                catch(const std::exception& e)
                {
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(str, startPos, L"Unknown json value format: " + numStr));
                }
                doc->setJsonInternalType(JsonInternalType::Number);
                doc->setJsonInternalValue(numStr);
            }
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(str, startPos, L"Parse json object error: " + str2wstr(e.what())));
        }
    }

    void JsonBuilder::deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const
    {
        TRY
            auto jsonObj = std::dynamic_pointer_cast<Json>(doc);
            assert(jsonObj != nullptr);
            getNextCharPos(str, pos, true);
            if (str[pos] != L'{')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(str, pos, L"Json Object not start with {"));
            getNextCharPos(str, pos, false);
            while (pos < str.length())
            {
                // name
                std::wstring name = getNextQuotedString(str, pos, {L":"}, { L"\"", L"'", L"{", L"["}, { L"\"", L"'", L"}", L"]"}, { L"\\", L"\\", L"\\", L"\\"}, { L"\"", L"'" });
                trim(name);
                if (isStartWith(name, L"\""))
                    name = getUnescapeStringWithQuote(EscapeStringType::DoubleQuote, name);
                else if (isStartWith(name, L"\'"))
                    name = getUnescapeStringWithQuote(EscapeStringType::SingleQuote, name);
                getNextCharPos(str, pos, false);
                if (str[pos] != L':')
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(str, pos, L"Json Object name " + name + L" not followed by :"));
                getNextCharPos(str, pos, false);

                // value
                auto obj = std::make_shared<Json>();
                ParseJsonObject(str, pos, obj);
                jsonObj->setJsonInternalType(JsonInternalType::Json);
                jsonObj->insertJsonInternalNameValuePairsAtKey(name, obj);

                getNextCharPos(str, pos, false);
                if (str[pos] != L',')
                    break;
                getNextCharPos(str, pos, false);
            }
        CATCH
    }

    void JsonBuilder::deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const
    {
        TRY
            size_t pos = 0;
            this->deserialize(str, pos, doc);
        CATCH
    }
}
