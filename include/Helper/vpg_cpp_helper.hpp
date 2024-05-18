#pragma once

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

// remove all command and special characters
inline std::wstring GetSimpleCode(const std::wstring &str) {
    TRY
        std::wstring result = L"";
        std::vector<std::wstring> prefixes = { L"/*", L"//" };
        std::vector<std::wstring> suffixes = { L"*/", L"\n"};
        for (size_t i = 0; i < str.size(); i++) {
            bool isCommand = false;
            for (size_t j = 0; j < prefixes.size(); j++) {
                if (HasPrefix(str, prefixes[j], i)) {
                    size_t pos = Find(str, suffixes[j], i + prefixes[j].size());
                    if (pos != std::wstring::npos)
                        i = pos + suffixes[j].size() - 1;
                    else
                        pos += str.size();
                    isCommand = true;
                    break;
                }
            }
            if (isCommand) 
                result += L" ";
            else
                result += std::wstring(1, str[i]);
        }
        return result;
    CATCH
    return L"";
}
