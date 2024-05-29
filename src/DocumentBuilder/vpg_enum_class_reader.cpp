#include "vpg_enum_class_reader.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

const std::wstring attributeToken = L"@@";

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
            if (IsStartWith(propertyCommand, str + L"(", pos)) {
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


std::vector<std::wstring> VPGEnumClassReader::GetAttribute(const std::wstring &str) const
{
    std::vector<std::wstring> result;
    TRY
        size_t pos = 0;
        GetNextCharPos(str, pos, true);

        if (!IsStartWith(str, attributeToken, pos))
            return result;
        
        std::vector<std::wstring> tokens = SplitString(str.substr(pos), { attributeToken });
        for (auto &token : tokens) {
            Trim(token);
            if (token.empty())
                continue;
            
            result.push_back(attributeToken + token);
        }
    CATCH
    return result;
}

void VPGEnumClassReader::_AssignEnumClassProperty(const std::wstring &propertyCommand, std::shared_ptr<VPGEnumClassProperty> property) const
{
    TRY
        size_t pos = 0;
        property->_Macro = _GetMacro(propertyCommand, pos);
        Trim(property->_Macro);
        
        std::wstring remainStr = property->_Macro.size() < propertyCommand.size() ? propertyCommand.substr(property->_Macro.size()) : L"";
        // if Macro is empty, then rollback to pos = 0
        if (property->_Macro.empty()) {
            property->_Command = remainStr;
            Trim(property->_Command);
            return;
        }

        // split macro
        pos = 0;
        if (IsStartWith(property->_Macro, L"MAP", pos)) {
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
        
        // split Remain
        pos = 0;
        std::vector<std::wstring> attributes = GetAttribute(remainStr);
        for (auto const &attribute : attributes) {
            if (Equal(attribute, attributeToken + L"ReadOnly", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::ReadOnly;
            else if (Equal(attribute, attributeToken + L"WriteOnly", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::WriteOnly;
            else if (Equal(attribute, attributeToken + L"ReadWrite", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::ReadWrite;
            else if (Equal(attribute, attributeToken + L"NoAccess", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::NoAccess;
        }
    CATCH
}

std::wstring VPGEnumClassReader::_GetCommand(const std::wstring &cppCode, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        GetNextCharPos(cppCode, pos, true);
        while (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
            std::wstring tmpCmd = GetNextQuotedString(cppCode, pos, { L";", L"{", L"\n", L" ", L"/*", L"//" }, { L"/*", L"//"}, {L"*/", L"\n"}, { L"", L"" });
            Trim(tmpCmd);
            if (!result.empty())
                result += L"\r\n";
            if (IsStartWith(tmpCmd, L"//")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 2); // only minus prefix as already trim
            } else if (IsStartWith(tmpCmd, L"/*")) {
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
            if (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
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
        if (!IsStartWith(cppCode, L"enum", pos))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"enum missing."));
            
        pos += 4; // length of "enum"
        GetNextCharPos(cppCode, pos, false);
        if (IsStartWith(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            GetNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, cppCode[pos], L"Class Name missing."));

        enumClass->_Name = _GetEnum(cppCode, pos);
        GetNextCharPos(cppCode, pos, false);

        if (IsStartWith(cppCode, L"//", pos)) {
            enumClass->_Command = _GetCommand(cppCode, pos);
            GetNextCharPos(cppCode, pos, false);
        } else if (IsStartWith(cppCode, L"/*", pos)) {
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
            if (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
                _GetCommand(cppCode, pos);
            } else if (IsStartWith(cppCode, L"enum", pos)) {
                DECLARE_SPTR(VPGEnumClass, enumClass);
                _ParseClass(cppCode, pos, enumClass);
                results.push_back(enumClass);
            }
            GetNextCharPos(cppCode, pos, false);
        }
    CATCH
}