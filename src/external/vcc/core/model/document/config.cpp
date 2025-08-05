#include "config.hpp"

#include <memory>
#include <string>

#include "exception_type.hpp"
#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    bool Config::isValue(const std::wstring &key) const
    {
        return !isBlank(key) && key != L"#";
    }

    std::wstring Config::getValue(const std::wstring &key) const
    {
        TRY
            if (isBlank(key) || key == L"#")
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"key is blank or #");
            return getConfigsAtKey(key);
        CATCH
        return L"";
    }

    void Config::AddValue(const std::wstring &key, const std::wstring &value)
    {
        TRY
            if (isBlank(key) || key == L"#")
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"key is blank or #");
            insertConfigsAtKey(key, value);
        CATCH
    }

    void Config::AddLine(const std::wstring &value)
    {
        TRY
            insertConfigsAtKey(L"", value);
        CATCH
    }

    void Config::AddCommand(const std::wstring &value)
    {
        TRY
            insertConfigsAtKey(L"#", value);
        CATCH
    }
}
