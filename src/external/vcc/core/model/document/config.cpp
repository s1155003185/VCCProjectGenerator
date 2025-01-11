#include "config.hpp"

#include <memory>
#include <string>

#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    bool Config::IsValue(const std::wstring &key) const
    {
        return !IsBlank(key) && key != L"#";
    }

    std::wstring Config::GetValue(const std::wstring &key) const
    {
        TRY
            if (IsBlank(key) || key == L"#")
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"key is blank or #");
            return GetConfigsAtKey(key);
        CATCH
        return L"";
    }

    void Config::AddValue(const std::wstring &key, const std::wstring &value) const
    {
        TRY
            if (IsBlank(key) || key == L"#")
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"key is blank or #");
            InsertConfigsAtKey(key, value);
        CATCH
    }

    void Config::AddLine(const std::wstring &value) const
    {
        TRY
            InsertConfigsAtKey(L"", value);
        CATCH
    }

    void Config::AddCommand(const std::wstring &value) const
    {
        TRY
            InsertConfigsAtKey(L"#", value);
        CATCH
    }
}
