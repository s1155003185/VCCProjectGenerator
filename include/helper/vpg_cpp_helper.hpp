#pragma once

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

// remove all command and special characters
inline std::wstring getSimpleCode(const std::wstring &str) {
    TRY
        std::wstring result = L"";
        std::vector<std::wstring> prefixes = { L"/*", L"//" };
        std::vector<std::wstring> suffixes = { L"*/", L"\n"};
        for (size_t i = 0; i < str.size(); i++) {
            bool isCommand = false;
            for (size_t j = 0; j < prefixes.size(); j++) {
                if (vcc::isStartWith(str, prefixes[j], i)) {
                    size_t pos = vcc::find(str, suffixes[j], i + prefixes[j].size());
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

inline std::wstring getIndentStringWithNamespaceLevel(const size_t &namespaceLevel) {
    TRY
        std::wstring indent = L"";
        for (size_t i = 0; i < namespaceLevel; i++)
            indent += L"    "; // 4 spaces
        return indent;
    CATCH
    return L"";
}

inline size_t getNamespaceLevel(const std::wstring &namespaceStr) {
    TRY
        if (namespaceStr.empty())
            return 0;
        return vcc::splitString(namespaceStr, { L"::" }).size();
    CATCH
    return 0;
}

inline std::wstring getNamespaceCommonPrefix(const std::wstring &namespace1, const std::wstring &namespace2) {
    TRY
        if (namespace1.empty() || namespace2.empty())
            return L"";
        auto nsTokens1 = vcc::splitString(namespace1, { L"::" });
        auto nsTokens2 = vcc::splitString(namespace2, { L"::" });
        size_t minSize = std::min(nsTokens1.size(), nsTokens2.size());
        std::wstring commonPrefix = L"";
        for (size_t i = 0; i < minSize; i++) {
            if (nsTokens1[i] == nsTokens2[i]) {
                if (!commonPrefix.empty()) {
                    commonPrefix += L"::";
                }
                commonPrefix += nsTokens1[i];
            } else
                break;
        }
        return commonPrefix;
    CATCH
    return L"";
}

inline std::wstring generateCodeWithNamespace(const std::map<std::wstring, std::vector<std::wstring>> &namespaceClassMapping)
{
    TRY
        std::wstring content = L"";
        std::wstring previousNamespace = L"";
        size_t namespaceLevel = 0;
        for (auto const &pair : namespaceClassMapping) {
            std::wstring commonPrefix = getNamespaceCommonPrefix(previousNamespace, pair.first);
            // Step 1: if previous namespace < common namespace, then close previous
            if (!previousNamespace.empty() && previousNamespace != commonPrefix) {
                // Close previous namespace
                namespaceLevel = getNamespaceLevel(previousNamespace);
                int64_t diff = std::abs((int64_t)namespaceLevel - (int64_t)getNamespaceLevel(commonPrefix));
                while (diff > 0) {
                    namespaceLevel--;
                    content += getIndentStringWithNamespaceLevel(namespaceLevel) + L"}\r\n";
                    diff--;
                }
            }
            // Step 2: if current namespace < common namespace, then create new namespace
            if (!pair.first.empty() && pair.first != commonPrefix) {
                size_t currentNamespaceLevel = getNamespaceLevel(commonPrefix);
                namespaceLevel = getNamespaceLevel(pair.first);
                std::wstring namespaceRemain = pair.first.substr(commonPrefix.length());
                if (vcc::isStartWith(namespaceRemain, L"::"))
                    namespaceRemain = namespaceRemain.substr(2);
                for (auto ns : vcc::splitString(namespaceRemain, { L"::" })) {
                    content += L"\r\n"
                        + getIndentStringWithNamespaceLevel(currentNamespaceLevel) + L"namespace " + ns + L"\r\n"
                        + getIndentStringWithNamespaceLevel(currentNamespaceLevel) + L"{\r\n";
                    currentNamespaceLevel++;
                }
            }
            previousNamespace = pair.first;
            namespaceLevel = getNamespaceLevel(previousNamespace);

            // Step 3: Generate class content
            bool isFirstClass = true;
            for (auto &classContent : pair.second) {
                std::wstring tmpStr = classContent;
                vcc::rTrim(tmpStr);
                auto lines = vcc::splitString(tmpStr, { L"\r\n" });
                for (auto &line : lines) {
                    if (isFirstClass && vcc::isBlank(line)) {
                        isFirstClass = false;
                        continue;
                    }
                    vcc::rTrim(line);
                    if (vcc::isBlank(content))
                        content += L"\r\n";
                    if (!vcc::isBlank(line))
                        content += getIndentStringWithNamespaceLevel(namespaceLevel) + line;
                    content += L"\r\n";
                }
            }
        }
        while (namespaceLevel > 0) {
            namespaceLevel--;
            content += getIndentStringWithNamespaceLevel(namespaceLevel) + L"}\r\n";
        }
        return content;
    CATCH
    return L"";
}