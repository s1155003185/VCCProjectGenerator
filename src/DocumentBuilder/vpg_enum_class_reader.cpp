#include "vpg_enum_class_reader.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

VPGEnumClassReader::VPGEnumClassReader(const std::set<std::wstring> &classMacroList) 
{
    this->_ClassMacroList.insert(classMacroList.begin(), classMacroList.end());
}

std::wstring VPGEnumClassReader::_GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg) const
{
    return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + std::wstring(1, c) + L"': " + msg;
}

std::wstring VPGEnumClassReader::_GetEnum(const std::wstring &propertyStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        GetNextCharPos(propertyStr, pos, true);
        while (pos < propertyStr.size())
        {
            if (!std::iswalpha(propertyStr[pos]) && !std::iswdigit(propertyStr[pos])) {
                pos--;
                break;
            }
            result += std::wstring(1, propertyStr[pos]);
            pos++;
        }
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetMacro(const std::wstring &propertyCommand, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        if (this->_ClassMacroList.empty())
            return result;

        GetNextCharPos(propertyCommand, pos, true);
        bool hasMacroPrefix = false;
        for (auto str : this->_ClassMacroList) {
            if (HasPrefix(propertyCommand, str + L"(", pos)) {
                hasMacroPrefix = true;
                break;
            }
        }
        if (!hasMacroPrefix) {
            pos = 0;
            return result;
        }
        
		while (pos < propertyCommand.length())
		{
			pos = Find(propertyCommand, L")", pos);
			if (pos == std::wstring::npos)
				break;
            if (CountSubstring(propertyCommand.substr(pos), L"\"") % 2 == 0) {
                result = propertyCommand.substr(0, pos + 1);
                break;
            }
			pos++;
		}
        Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetType(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        pos = Find(macroStr, L"(");
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetType: Macro ( missing");
        pos++;
        size_t endPos = Find(macroStr, L",");
        if (endPos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetType: Macro , missing");
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetPropertyName(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = Find(macroStr, L",", pos);
        if (endPos == std::wstring::npos) {
            endPos = macroStr.find_last_of(L")");
            if (endPos == std::wstring::npos) 
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetPropertyName: Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetDefaultValue(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = macroStr.find_last_of(L")");
        if (endPos == std::wstring::npos) 
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetDefaultValue: Macro ) missing");

        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    CATCH
    return result;
}

void VPGEnumClassReader::_AssignEnumClassProperty(const std::wstring &propertyCommand, std::shared_ptr<VPGEnumClassProperty> property) const
{
    TRY
        size_t pos = 0;
        property->_Macro = _GetMacro(propertyCommand, pos);
        Trim(property->_Macro);

        pos++;
        // if Macro is empty, then rollback to pos = 0
        if (property->_Macro.empty())
            pos = 0;

        property->_Command = pos < propertyCommand.size() ? propertyCommand.substr(pos) : L"";
        Trim(property->_Command);

        // split macro
        if (!property->_Macro.empty()) {
            pos = 0;
            if (HasPrefix(property->_Macro, L"MAP", pos)) {
                property->_Type1 = _GetType(property->_Macro, pos);
                if (property->_Macro[pos] != L',')
                    return;
                pos++;
                property->_Type2 = _GetPropertyName(property->_Macro, pos);
                if (property->_Macro[pos] != L',')
                    return;
                pos++;
                property->_PropertyName = _GetDefaultValue(property->_Macro, pos);
            } else {
                property->_Type1 = _GetType(property->_Macro, pos);
                if (property->_Macro[pos] != L',')
                    return;
                pos++;
                property->_PropertyName = _GetPropertyName(property->_Macro, pos);
                if (property->_Macro[pos] != L',')
                    return;
                pos++;
                property->_DefaultValue = _GetDefaultValue(property->_Macro, pos);
            }
        }
    CATCH
}

std::wstring VPGEnumClassReader::_GetCommand(const std::wstring &cppCode, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        GetNextCharPos(cppCode, pos, true);
        while (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
            std::wstring tmpCmd = GetNextQuotedString(cppCode, pos, { L";", L"{", L"\n", L" ", L"/*", L"//" }, { L"/*", L"//"}, {L"*/", L"\n"}, { L"", L"" });
            Trim(tmpCmd);
            if (!result.empty())
                result += L"\r\n";
            if (HasPrefix(tmpCmd, L"//")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 2); // only minus prefix as already trim
            } else if (HasPrefix(tmpCmd, L"/*")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 4);
            }
            Trim(tmpCmd);
            result += tmpCmd;
            GetNextCharPos(cppCode, pos, false);
        }
        Trim(result);
        // need to back 1 char as used GetNextCharPos
        pos--;
    CATCH
    return result;
}

void VPGEnumClassReader::_ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const
{
    TRY
        while (pos < cppCode.size()) {
            std::wstring name = _GetEnum(cppCode, pos);
            Trim(name);
            if (name.empty())
                break;
            
            DECLARE_SPTR(VPGEnumClassProperty, property);
            property->_Enum = name;
            GetNextCharPos(cppCode, pos, false);
            if (cppCode[pos] == L'=') {
                GetNextCharPos(cppCode, pos, false);
                GetNextCharPos(cppCode, pos, false);
            }
            if (cppCode[pos] == L',')
                GetNextCharPos(cppCode, pos, false);
            if (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
                _AssignEnumClassProperty(_GetCommand(cppCode, pos), property);
                GetNextCharPos(cppCode, pos, false);
            }

            enumClass->_Properties.push_back(property);

            if (cppCode[pos] == L'}') {
                pos--;
                return;
            }
        }
    CATCH
}

void VPGEnumClassReader::_ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const
{
    TRY
        if (!HasPrefix(cppCode, L"enum", pos))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"enum missing."));
            
        pos += 4; // length of "enum"
        GetNextCharPos(cppCode, pos, false);
        if (HasPrefix(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            GetNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"Class Name missing."));

        enumClass->_Name = _GetEnum(cppCode, pos);
        GetNextCharPos(cppCode, pos, false);

        if (HasPrefix(cppCode, L"//", pos)) {
            enumClass->_Command = _GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        } else if (HasPrefix(cppCode, L"/*", pos)) {
            enumClass->_Command =_GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        }
        if (cppCode[pos] != L'{')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"{ missing."));
        GetNextCharPos(cppCode, pos, false);

        _ParseProperties(cppCode, pos, enumClass);
        GetNextCharPos(cppCode, pos, false);

        if (cppCode[pos] != L'}')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"} missing."));
    CATCH
}

void VPGEnumClassReader::Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        size_t pos = 0;
        while (pos < cppCode.size()) {
            if (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
                _GetCommand(cppCode, pos);
            } else if (HasPrefix(cppCode, L"enum", pos)) {
                DECLARE_SPTR(VPGEnumClass, enumClass);
                _ParseClass(cppCode, pos, enumClass);
                results.push_back(enumClass);
            }
            GetNextCharPos(cppCode, pos, false);
        }
    CATCH
}