#include "vpg_include_path_reader.hpp"

#include "exception_macro.hpp"
#include "string_helper.hpp"

size_t VPGIncludePathReader::IsQuote(const std::wstring &cppCode, const size_t &pos) const
{
    TRY
        size_t index = 0;
        for (auto const &quote : _OpenQuotes) {
            if (vcc::IsStartWith(cppCode, quote, pos))
                return index;
            index++;
        }
    CATCH
    return std::wstring::npos;
}

size_t VPGIncludePathReader::IsCommand(const std::wstring &cppCode, const size_t &pos) const
{
    TRY
        size_t index = 0;
        for (auto const &quote : _OpenCommands) {
            if (vcc::IsStartWith(cppCode, quote, pos))
                return index;
            index++;
        }
    CATCH
    return std::wstring::npos;
}

size_t VPGIncludePathReader::SkipCommand(const std::wstring &cppCode, const size_t &openCommandIndex, size_t &pos) const
{
    TRY
        std::wstring closeCommand = _CloseCommands[openCommandIndex];
        size_t tmpPos = vcc::Find(cppCode, closeCommand, pos);
        if (tmpPos != std::wstring::npos)
            pos = tmpPos + closeCommand.length() - 1;
    CATCH
    return false;
}

void VPGIncludePathReader::ParseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::set<std::wstring> &classList) const
{
    if (cppCode.empty())
        return;
    TRY
        size_t pos = 0;
        bool isNamespaceTriggered = false;
        vcc::GetNextCharPos(cppCode, pos, true);
        while (pos < cppCode.length()) {
            size_t commandIndex = IsCommand(cppCode, pos);
            if (commandIndex != std::wstring::npos) {
                SkipCommand(cppCode, commandIndex, pos);
            } else {
                std::wstring nextToken = vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                if (!vcc::IsBlank(nextToken)) {
                    if (isNamespaceTriggered) {
                        // namespace name
                        vcc::Trim(nextToken);
                        pos++;
                        std::wstring quoteStr = vcc::GetNextQuotedString(cppCode, pos, { L";" } );
                        vcc::Trim(quoteStr);
                        if (!quoteStr.empty()) {
                            std::set<std::wstring> tmpClassList;
                            ParseCustom(quoteStr.substr(1, quoteStr.length() - 2), nextToken, tmpClassList);
                            if (!currentNamespace.empty()) {
                                for (auto const &tmpClass : tmpClassList)
                                    classList.insert(currentNamespace + L"::" + tmpClass);
                            } else
                                classList.insert(tmpClassList.begin(), tmpClassList.end());
                        }
                        isNamespaceTriggered = false;
                    } else {
                        if (nextToken == L"namespace") {
                            isNamespaceTriggered = true;
                        } else {
                            if (nextToken == L"class") {
                                vcc::GetNextCharPos(cppCode, pos, false);
                                nextToken = vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::Trim(nextToken);
                                vcc::GetNextCharPos(cppCode, pos, false);
                                // check if it is :
                                if (cppCode[pos] == L';')
                                    continue;
                                if (cppCode[pos] == L':') {
                                    pos = vcc::Find(cppCode, L"{", pos);
                                }
                                // drop class quote
                                if (!vcc::GetNextQuotedString(cppCode, pos, { L";" }).empty())
                                    classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                            } else if (nextToken == L"typedef") {
                                vcc::GetNextCharPos(cppCode, pos, false);
                                vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::GetNextCharPos(cppCode, pos, false);
                                nextToken = vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::Trim(nextToken);
                                classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                            } else if (nextToken == L"using") {
                                vcc::GetNextCharPos(cppCode, pos, false);
                                nextToken = vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::Trim(nextToken);
                                vcc::GetNextCharPos(cppCode, pos, false);
                                // check if it is :
                                if (cppCode[pos] == L';')
                                    continue;
                                else if (cppCode[pos] == L'=') {
                                    classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                                }
                                pos = vcc::Find(cppCode, L";", pos);
                            }
                        }
                    }
                }
            }
            vcc::GetNextCharPos(cppCode, pos, false);
        }
    CATCH
}

// void VPGIncludePathReader::ParseLinux(const std::wstring &cppCode, std::set<std::wstring> &classList) const
// {
//     if (cppCode.empty())
//         return;
//     TRY_CATCH () {

//     }
// }

// void VPGIncludePathReader::ParseMacOs(const std::wstring &cppCode, std::set<std::wstring> &classList) const
// {
//     if (cppCode.empty())
//         return;
//     TRY_CATCH () {
//         size_t startCommandPos = vcc::Find(cppCode, L"/*");
//         if (startCommandPos == std::wstring::npos)
//             return;

//         size_t endCommandPos = vcc::Find(cppCode, L"*/", startCommandPos);
//         if (endCommandPos == std::wstring::npos)
//             return;

//         std::wstring token = L"synopsis";
//         startCommandPos = vcc::Find(cppCode, token, startCommandPos);
//         if (startCommandPos == std::wstring::npos || endCommandPos < startCommandPos)
//             return;

//         startCommandPos += token.length();
//         ParseCustom(cppCode.substr(startCommandPos, endCommandPos - startCommandPos), L"", classList);       
//     }
// }

// void VPGIncludePathReader::ParseWindow(const std::wstring &cppCode, std::set<std::wstring> &classList) const
// {
//     if (cppCode.empty())
//         return;
//     TRY_CATCH () {
//         std::wstring tmpCode = cppCode;
//         ReplaceAll(tmpCode, L"_STD_BEGIN", L"namespace std {");
//         ReplaceAll(tmpCode, L"_STD_END", L"}");
//         ParseCustom(tmpCode, L"", classList);  
//     }
// }

void VPGIncludePathReader::Parse(const vcc::PlatformType &platformType, const std::wstring &cppCode, std::set<std::wstring> &classList) const
{
    TRY
        switch (platformType)
        {
        case vcc::PlatformType::NA:
            ParseCustom(cppCode, L"", classList);
            break;
        // case vcc::PlatformType::Linux:
        //     ParseLinux(cppCode, classList);
        //     break;
        // case vcc::PlatformType::MacOs:
        //     ParseMacOs(cppCode, classList);
        //     break;
        // case vcc::PlatformType::Window:
        //     ParseWindow(cppCode, classList);
        //     break;        
        default:
            assert(false);
            break;
        }
    CATCH
}