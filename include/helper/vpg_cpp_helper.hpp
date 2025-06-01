#pragma once

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

// remove all command and special characters
inline std::wstring GetSimpleCode(const std::wstring &str) {
    TRY
        std::wstring result = L"";
        std::vector<std::wstring> prefixes = { L"/*", L"//" };
        std::vector<std::wstring> suffixes = { L"*/", L"\n"};
        for (size_t i = 0; i < str.size(); i++) {
            bool isCommand = false;
            for (size_t j = 0; j < prefixes.size(); j++) {
                if (vcc::IsStartWith(str, prefixes[j], i)) {
                    size_t pos = vcc::Find(str, suffixes[j], i + prefixes[j].size());
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

inline std::wstring GetIndentStringWithNamespaceLevel(const size_t &namespaceLevel) {
    TRY
        std::wstring indent = L"";
        for (size_t i = 0; i < namespaceLevel; i++)
            indent += L"    "; // 4 spaces
        return indent;
    CATCH
    return L"";
}

inline size_t GetNamespaceLevel(const std::wstring &namespaceStr) {
    TRY
        if (namespaceStr.empty())
            return 0;
        return vcc::SplitString(namespaceStr, { L"::" }).size();
    CATCH
    return 0;
}

inline std::wstring GetNamespaceCommonPrefix(const std::wstring &namespace1, const std::wstring &namespace2) {
    TRY
        if (namespace1.empty() || namespace2.empty())
            return L"";
        auto nsTokens1 = vcc::SplitString(namespace1, { L"::" });
        auto nsTokens2 = vcc::SplitString(namespace2, { L"::" });
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

inline std::wstring GenerateCodeWithNamespace(const std::map<std::wstring, std::vector<std::wstring>> &namespaceClassMapping)
{
    TRY
        std::wstring content = L"";
        std::wstring previousNamespace = L"";
        size_t namespaceLevel = 0;
        for (auto const &pair : namespaceClassMapping) {
            std::wstring commonPrefix = GetNamespaceCommonPrefix(previousNamespace, pair.first);
            // Step 1: if previous namespace < common namespace, then close previous
            if (!previousNamespace.empty() && previousNamespace != commonPrefix) {
                // Close previous namespace
                namespaceLevel = GetNamespaceLevel(previousNamespace);
                int64_t diff = std::abs((int64_t)namespaceLevel - (int64_t)GetNamespaceLevel(commonPrefix));
                while (diff > 0) {
                    namespaceLevel--;
                    content += GetIndentStringWithNamespaceLevel(namespaceLevel) + L"}\r\n";
                    diff--;
                }
            }
            // Step 2: if current namespace < common namespace, then create new namespace
            if (!pair.first.empty() && pair.first != commonPrefix) {
                size_t currentNamespaceLevel = GetNamespaceLevel(commonPrefix);
                namespaceLevel = GetNamespaceLevel(pair.first);
                std::wstring namespaceRemain = pair.first.substr(commonPrefix.length());
                if (vcc::IsStartWith(namespaceRemain, L"::"))
                    namespaceRemain = namespaceRemain.substr(2);
                for (auto ns : vcc::SplitString(namespaceRemain, { L"::" })) {
                    content += L"\r\n"
                        + GetIndentStringWithNamespaceLevel(currentNamespaceLevel) + L"namespace " + ns + L"\r\n"
                        + GetIndentStringWithNamespaceLevel(currentNamespaceLevel) + L"{\r\n";
                    currentNamespaceLevel++;
                }
            }
            previousNamespace = pair.first;
            namespaceLevel = GetNamespaceLevel(previousNamespace);

            // Step 3: Generate class content
            bool isFirstClass = true;
            for (auto &classContent : pair.second) {
                std::wstring tmpStr = classContent;
                vcc::RTrim(tmpStr);
                auto lines = vcc::SplitString(tmpStr, { L"\r\n" });
                for (auto &line : lines) {
                    if (isFirstClass && vcc::IsBlank(line)) {
                        isFirstClass = false;
                        continue;
                    }
                    vcc::RTrim(line);
                    if (vcc::IsBlank(content))
                        content += L"\r\n";
                    if (!vcc::IsBlank(line))
                        content += GetIndentStringWithNamespaceLevel(namespaceLevel) + line;
                    content += L"\r\n";
                }
            }
        }
        while (namespaceLevel > 0) {
            namespaceLevel--;
            content += GetIndentStringWithNamespaceLevel(namespaceLevel) + L"}\r\n";
        }
        return content;
    CATCH
    return L"";
}