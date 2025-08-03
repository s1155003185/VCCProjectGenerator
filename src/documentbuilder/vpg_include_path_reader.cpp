#include "vpg_include_path_reader.hpp"

#include "exception_macro.hpp"
#include "string_helper.hpp"

size_t VPGIncludePathReader::IsQuote(const std::wstring &cppCode, const size_t &pos) const
{
    TRY
        size_t index = 0;
        for (auto const &quote : _OpenQuotes) {
            if (vcc::isStartWith(cppCode, quote, pos))
                return index;
            index++;
        }
    CATCH
    return std::wstring::npos;
}

size_t VPGIncludePathReader::isCommand(const std::wstring &cppCode, const size_t &pos) const
{
    TRY
        size_t index = 0;
        for (auto const &quote : _OpenCommands) {
            if (vcc::isStartWith(cppCode, quote, pos))
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
        size_t tmpPos = vcc::find(cppCode, closeCommand, pos);
        if (tmpPos != std::wstring::npos)
            pos = tmpPos + closeCommand.length() - 1;
    CATCH
    return false;
}

void VPGIncludePathReader::parseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::set<std::wstring> &classList) const
{
    if (cppCode.empty())
        return;
    TRY
        size_t pos = 0;
        bool isNamespaceTriggered = false;
        vcc::getNextCharPos(cppCode, pos, true);
        while (pos < cppCode.length()) {
            size_t commandIndex = isCommand(cppCode, pos);
            if (commandIndex != std::wstring::npos) {
                SkipCommand(cppCode, commandIndex, pos);
            } else {
                std::wstring nextToken = vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                if (!vcc::isBlank(nextToken)) {
                    if (isNamespaceTriggered) {
                        // namespace name
                        vcc::trim(nextToken);
                        pos++;
                        std::wstring quoteStr = vcc::getNextQuotedString(cppCode, pos, { L";" } );
                        vcc::trim(quoteStr);
                        if (!quoteStr.empty()) {
                            std::set<std::wstring> tmpClassList;
                            parseCustom(quoteStr.substr(1, quoteStr.length() - 2), nextToken, tmpClassList);
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
                                vcc::getNextCharPos(cppCode, pos, false);
                                nextToken = vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::trim(nextToken);
                                vcc::getNextCharPos(cppCode, pos, false);
                                // check if it is :
                                if (cppCode[pos] == L';')
                                    continue;
                                if (cppCode[pos] == L':') {
                                    pos = vcc::find(cppCode, L"{", pos);
                                }
                                // drop class quote
                                if (!vcc::getNextQuotedString(cppCode, pos, { L";" }).empty())
                                    classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                            } else if (nextToken == L"typedef") {
                                vcc::getNextCharPos(cppCode, pos, false);
                                vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::getNextCharPos(cppCode, pos, false);
                                nextToken = vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::trim(nextToken);
                                classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                            } else if (nextToken == L"using") {
                                vcc::getNextCharPos(cppCode, pos, false);
                                nextToken = vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                                vcc::trim(nextToken);
                                vcc::getNextCharPos(cppCode, pos, false);
                                // check if it is :
                                if (cppCode[pos] == L';')
                                    continue;
                                else if (cppCode[pos] == L'=') {
                                    classList.insert((!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + nextToken);
                                }
                                pos = vcc::find(cppCode, L";", pos);
                            }
                        }
                    }
                }
            }
            vcc::getNextCharPos(cppCode, pos, false);
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
//         size_t startCommandPos = vcc::find(cppCode, L"/*");
//         if (startCommandPos == std::wstring::npos)
//             return;

//         size_t endCommandPos = vcc::find(cppCode, L"*/", startCommandPos);
//         if (endCommandPos == std::wstring::npos)
//             return;

//         std::wstring token = L"synopsis";
//         startCommandPos = vcc::find(cppCode, token, startCommandPos);
//         if (startCommandPos == std::wstring::npos || endCommandPos < startCommandPos)
//             return;

//         startCommandPos += token.length();
//         parseCustom(cppCode.substr(startCommandPos, endCommandPos - startCommandPos), L"", classList);       
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
//         parseCustom(tmpCode, L"", classList);  
//     }
// }

void VPGIncludePathReader::parse(const vcc::PlatformType &platformType, const std::wstring &cppCode, std::set<std::wstring> &classList) const
{
    TRY
        switch (platformType)
        {
        case vcc::PlatformType::NA:
            parseCustom(cppCode, L"", classList);
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