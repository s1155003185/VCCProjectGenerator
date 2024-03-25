#include "json_builder.hpp"

#include <string>

#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "json.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring JsonBuilder::Serialize(const Json *doc)
    {
        return L"";
    }

    std::wstring JsonBuilder::GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
    {
        return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
    }

    void JsonBuilder::ParseJsonObject(const std::wstring &str, size_t &pos, std::shared_ptr<Json> doc)
    {
        //TRY_CATCH(
            GetNextCharPos(str, pos, true);
            if (str[pos] != L'{')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(pos, str[pos], L"Json Object not start with {"));
            while (pos < str.length())
            {
                pos++;
            }
        //)
    }

    void JsonBuilder::Deserialize(const std::wstring &str, std::shared_ptr<Json> doc)
    {
        TRY_CATCH(
            size_t pos = 0;
            ParseJsonObject(str, pos, doc);
        )
    }
}
