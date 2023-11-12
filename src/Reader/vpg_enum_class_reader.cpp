#include "vpg_enum_class_reader.hpp"

#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

VPGEnumClassReader::VPGEnumClassReader(const std::set<std::wstring> &classMacroList) 
{
    this->_ClassMacroList = std::set(classMacroList.begin(), classMacroList.end());
}

std::wstring VPGEnumClassReader::_GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
{
    return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
}

std::wstring VPGEnumClassReader::_GetEnum(const std::wstring &propertyStr, size_t &pos)
{
    std::wstring result = L"";
    try
    {
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
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

std::wstring VPGEnumClassReader::_GetMacro(const std::wstring &propertyCommand, size_t &pos)
{
    std::wstring result = L"";
    try
    {
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
			pos = propertyCommand.find(L")", pos);
			if (pos == std::wstring::npos)
				break;
            if (CountSubstr(propertyCommand.substr(pos), L"\"") % 2 == 0) {
                result = propertyCommand.substr(0, pos + 1);
                break;
            }
			pos++;
		}
        Trim(result);
    } 
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

std::wstring VPGEnumClassReader::_GetType(const std::wstring &macroStr, size_t &pos)
{
    std::wstring result = L"";
    try
    {
        pos = macroStr.find(L"(");
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, L"GetType: Macro ( missing");
        pos++;
        size_t endPos = macroStr.find(L",");
        if (endPos == std::wstring::npos)
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, L"GetType: Macro , missing");
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    } 
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

std::wstring VPGEnumClassReader::_GetPropertyName(const std::wstring &macroStr, size_t &pos)
{
    std::wstring result = L"";
    try
    {
        size_t endPos = macroStr.find(L",", pos);
        if (endPos == std::wstring::npos) {
            endPos = macroStr.find_last_of(L")");
            if (endPos == std::wstring::npos) 
                THROW_EXCEPTION_M(ExceptionType::READER_ERROR, L"GetPropertyName: Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    } 
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

std::wstring VPGEnumClassReader::_GetDefaultValue(const std::wstring &macroStr, size_t &pos)
{
    std::wstring result = L"";
    try
    {
        size_t endPos = macroStr.find_last_of(L")");
        if (endPos == std::wstring::npos) 
            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, L"GetDefaultValue: Macro ) missing");

        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        Trim(result);
    } 
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}

void VPGEnumClassReader::_AssignEnumClassProperty(const std::wstring &propertyCommand, VPGEnumClassProperty &property)
{
    try
    {
        size_t pos = 0;
        property.Macro = _GetMacro(propertyCommand, pos);
        Trim(property.Macro);

        pos++;
        // if Macro is empty, then rollback to pos = 0
        if (property.Macro.empty())
            pos = 0;

        property.Command = pos < propertyCommand.size() ? propertyCommand.substr(pos) : L"";
        Trim(property.Command);

        // split macro
        if (!property.Macro.empty()) {
            pos = 0;
            property.Type = _GetType(property.Macro, pos);
            if (property.Macro[pos] != L',')
                return;
            pos++;
            property.PropertyName = _GetPropertyName(property.Macro, pos);
            if (property.Macro[pos] != L',')
                return;
            pos++;
            property.DefaultValue = _GetDefaultValue(property.Macro, pos);
        }
    } 
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
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
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    Trim(result);
    return result;
}

void VPGEnumClassReader::_ParseProperties(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass)
{
    try
    {
        while (pos < cppCode.size()) {
            std::wstring name = _GetEnum(cppCode, pos);
            Trim(name);
            if (name.empty())
                break;
            
            VPGEnumClassProperty property;
            property.Enum = name;
            GetNextCharPos(cppCode, pos, false);
            if (cppCode[pos] == L',')
                GetNextCharPos(cppCode, pos, false);
            if (HasPrefix(cppCode, L"//", pos) || HasPrefix(cppCode, L"/*", pos)) {
                _AssignEnumClassProperty(_GetCommand(cppCode, pos), property);
                GetNextCharPos(cppCode, pos, false);
            }

            enumClass.Properties.push_back(property);

            if (cppCode[pos] == L'}') {
                pos--;
                return;
            }
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
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

        enumClass.Name = _GetEnum(cppCode, pos);
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
    catch(const std::exception& e)
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
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}