#pragma once

#include <string>
#include <vector>
#include <set>

#include "platform_type.hpp"

using namespace vcc;

class VPGIncludePathReader
{
    private:
        std::vector<std::wstring> _OpenCommands = { L"/*", L"//"};
        std::vector<std::wstring> _CloseCommands = { L"*/", L"\n" };
        std::vector<std::wstring> _OpenQuotes = { L"{", L"[" };
        std::vector<std::wstring> _CloseQuotes = { L"}", L"]"};
        std::vector<std::wstring> _OpenCommandAndQuotes = { L"/*", L"//", L"{", L"<<", L"<" };
        std::vector<std::wstring> _CloseCommandAndQuotes = { L"*/", L"\n", L"}", L";", L">" };
        std::vector<std::wstring> _Delimiter = { L" ", L"\t", L"\n", L",", L";", L"{", L"}" };

        size_t IsQuote(const std::wstring &cppCode, const size_t &pos) const;
        size_t IsCommand(const std::wstring &cppCode, const size_t &pos) const;
        size_t SkipCommand(const std::wstring &cppCode, const size_t &openCommandIndex, size_t &pos) const;

        void ParseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::set<std::wstring> &classList) const;
        // void ParseLinux(const std::wstring &cppCode, std::set<std::wstring> &classList) const;
        // void ParseMacOs(const std::wstring &cppCode, std::set<std::wstring> &classList) const;
        // void ParseWindow(const std::wstring &cppCode, std::set<std::wstring> &classList) const;

    public:
        VPGIncludePathReader() = default;
        ~VPGIncludePathReader() {}

        void Parse(const PlatformType &platformType, const std::wstring &cppCode, std::set<std::wstring> &classList) const;
};