#include "vpg_enum_class_reader.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

std::wstring VPGEnumClassReader::_GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
{
    return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
}

std::wstring VPGEnumClassReader::_GetName(const std::wstring &cppCode, size_t &pos)
{
    std::wstring result = L"";
    try
    {
        GetNextCharPos(cppCode, pos, true);
        while (pos < cppCode.size())
        {
            if (!std::iswalpha(cppCode[pos]) && !std::iswdigit(cppCode[pos])) {
                pos--;
                break;
            }
            result += std::wstring(1, cppCode[pos]);
            pos++;
        }
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

std::wstring VPGEnumClassReader::_GetCommand(const std::wstring &cppCode, size_t &pos)
{
    std::wstring result = L"";
    try
    {
        if (HasPrefix(cppCode, L"//", pos)) {
            pos++;
            pos++;
            size_t endPos = cppCode.find_first_of(L"\n", pos);
            if (endPos == string::npos)
                endPos = cppCode.length() - 1;
            result = cppCode.substr(pos, endPos - pos - 1);
            pos = endPos;
        } else if (HasPrefix(cppCode, L"/*", pos)) {
            pos++;
            pos++;
            size_t endPos = cppCode.find_first_of(L"*/", pos);
            if (endPos == string::npos)
                endPos = cppCode.length() - 1;
            result = cppCode.substr(pos, endPos - pos);
            pos = endPos + 1;
        } else
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, cppCode[pos], L"// or /* missing."));    
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    Trim(result);
    return result;
}

void VPGEnumClassReader::_ParseProperties(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass)
{
    while (pos < cppCode.size()) {
        std::wstring name = _GetName(cppCode, pos);
        Trim(name);
        if (name.empty())
            break;
        
        VPGEnumClassProperty property;
        property.Name = name;
        GetNextCharPos(cppCode, pos, false);
        if (cppCode[pos] == L',')
            GetNextCharPos(cppCode, pos, false);
        if (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
            property.Command = _GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        }
        enumClass.Properties.push_back(property);

        if (cppCode[pos] == L'}') {
            pos--;
            return;
        }
    }
}

void VPGEnumClassReader::_ParseClass(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass)
{
    try
    {
        if (!HasPrefix(cppCode, L"enum", pos))
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, cppCode[pos], L"enum missing."));
            
        pos += 4; // length of "enum"
        GetNextCharPos(cppCode, pos, false);
        if (HasPrefix(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            GetNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, cppCode[pos], L"Class Name missing."));

        enumClass.Name = _GetName(cppCode, pos);
        GetNextCharPos(cppCode, pos, false);

        if (HasPrefix(cppCode, L"//", pos)) {
            enumClass.Command = _GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        } else if (HasPrefix(cppCode, L"/*", pos)) {
            enumClass.Command =_GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        }
        if (cppCode[pos] != L'{')
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, cppCode[pos], L"{ missing."));
        GetNextCharPos(cppCode, pos, false);

        _ParseProperties(cppCode, pos, enumClass);
        GetNextCharPos(cppCode, pos, false);

        if (cppCode[pos] != L'}')
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, cppCode[pos], L"} missing."));
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}

void VPGEnumClassReader::Parse(const std::wstring &cppCode, std::vector<VPGEnumClass> &results)
{
    try
    {
        size_t pos = 0;
        while (pos < cppCode.size()) {
            if (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
                _GetCommand(cppCode, pos);
            } else if (HasPrefix(cppCode, L"enum", pos)) {
                VPGEnumClass enumClass;
                _ParseClass(cppCode, pos, enumClass);
                results.push_back(enumClass);
            }
            pos++;
        }
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}