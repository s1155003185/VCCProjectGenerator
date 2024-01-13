#include "config_reader.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    void ConfigReader::Parse(const std::wstring &str, std::shared_ptr<ConfigElement> configElement)
    {
        TRY_CATCH(
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (std::wstring line : lines) {
                Trim(line);
                if (HasPrefix(line, L"#"))
                    continue;
                size_t pos = line.find(L"=");
                if (pos == std::wstring::npos)
                    continue;
                std::wstring key = line.substr(0, pos);
                std::wstring value = line.substr(pos + 1);
                Trim(key);
                Trim(value);
                configElement->_Configs->insert({key, value});
            }
        )
    }
}