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
    std::wstring ConfigBuilder::Serialize(const IDocument *doc) const
    {
        std::wstring result = L"";
        TRY_CATCH(){  
            Config *configObj = dynamic_cast<Config *>(const_cast<IDocument *>(doc));
            assert(configObj != nullptr);
            for (auto const &pair : configObj->GetConfigs()) {
                std::wstring key = pair.first;
                std::wstring value = pair.second;
                Trim(key);
                Trim(value);
                if (configObj->IsValue(key))
                    result += key + L"=" + value + L"\r\n";
                else
                    result += value + L"\r\n";
            }
        }
        return result;
    }

    void ConfigBuilder::Deserialize(const std::wstring &str, size_t &/*pos*/, std::shared_ptr<IDocument> doc) const
    {
        TRY_CATCH(){
            std::shared_ptr<Config> configObj = dynamic_pointer_cast<Config>(doc);
            assert(configObj != nullptr);

            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (std::wstring line : lines) {
                Trim(line);

                if (line.empty())
                    configObj->AddLine();
                else if (HasPrefix(line, L"#"))
                    configObj->AddCommand(line);
                else {
                    size_t eqPos = line.find(L"=");
                    if (eqPos != std::wstring::npos) {
                        std::wstring key = line.substr(0, eqPos);
                        std::wstring value = line.substr(eqPos + 1);
                        Trim(key);
                        Trim(value);
                        configObj->AddValue(key, value);
                    } else
                        configObj->AddLine(line);
                }
            }
        }
    }

    void ConfigBuilder::Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const
    {
        TRY_CATCH(){
            size_t pos = 0;
            Deserialize(str, pos, doc);
        }
    }
}