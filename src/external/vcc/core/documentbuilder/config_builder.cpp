#include "config_builder.hpp"

#include <assert.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "config.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring ConfigBuilder::serialize(const IDocument *doc) const
    {
        std::wstring result = L"";
        TRY  
            Config *configObj = dynamic_cast<Config *>(const_cast<IDocument *>(doc));
            assert(configObj != nullptr);
            for (auto const &pair : configObj->getConfigs()) {
                std::wstring key = pair.first;
                std::wstring value = pair.second;
                trim(key);
                trim(value);
                if (configObj->isValue(key))
                    result += key + L"=" + value + L"\r\n";
                else
                    result += value + L"\r\n";
            }
        CATCH
        return result;
    }

    void ConfigBuilder::deserialize(const std::wstring &str, size_t &/*pos*/, std::shared_ptr<IDocument> doc) const
    {
        TRY
            auto configObj = std::dynamic_pointer_cast<Config>(doc);
            assert(configObj != nullptr);

            std::vector<std::wstring> lines = splitStringByLine(str);
            for (std::wstring line : lines) {
                trim(line);

                if (line.empty())
                    configObj->AddLine();
                else if (isStartWith(line, L"#"))
                    configObj->AddCommand(line);
                else {
                    size_t eqPos = find(line, L"=");
                    if (eqPos != std::wstring::npos) {
                        std::wstring key = line.substr(0, eqPos);
                        std::wstring value = line.substr(eqPos + 1);
                        trim(key);
                        trim(value);
                        configObj->AddValue(key, value);
                    } else
                        configObj->AddLine(line);
                }
            }
        CATCH
    }

    void ConfigBuilder::deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const
    {
        TRY
            size_t pos = 0;
            deserialize(str, pos, doc);
        CATCH
    }
}