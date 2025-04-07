#include "vpg_enum_class_reader.hpp"

#include <assert.h>
#include <math.h>
#include <string>
#include <mutex>
#include <vector>

#include "exception_macro.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "string_helper.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

const std::wstring attributePrefix = L"@@";
std::mutex _mutex;

VPGEnumClassReader::VPGEnumClassReader(const std::set<std::wstring> &classMacroList) 
{
    this->_ClassMacroList.insert(classMacroList.begin(), classMacroList.end());
}

std::wstring VPGEnumClassReader::_GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const
{
        size_t row = 0, column = 0;
        GetCharacterRowAndColumn(str, pos, row, column);
        
        size_t lengthOfPos = std::min(pos, (size_t)100);
        size_t lengthOfSub = std::min(str.length() - pos, (size_t)100);

        std::wstring preString = str.substr(pos - lengthOfPos, lengthOfPos);
        std::wstring subString = str.substr(pos + 1, lengthOfSub);
        return L"Error at position " + std::to_wstring(pos + 1) 
            + L", row " + std::to_wstring(row) 
            + L", column " + std::to_wstring(column) + L": " + preString + L"<<" + std::wstring(1, str[pos]) + L">>" + subString + L"\r\n "
            + L" Error message: " + msg;
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
        if (endPos == std::wstring::npos) {
            endPos = Find(macroStr, L")");
            if (endPos == std::wstring::npos)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetType: Macro , or ) missing");
        }
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

std::shared_ptr<Json> VPGEnumClassReader::GetJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const
{
    TRY
        size_t pos = Find(command, attributeName, 0, true);
        if (pos == std::wstring::npos)
            return nullptr;
        pos += attributeName.length();
        GetNextCharPos(command, pos, true);
        if (command[pos] == L'{') {
            std::wstring jsonStr = GetNextQuotedString(command, pos, { L" ", L"\t", L"\n" }, { L"{", L"[", L"\"", L"'" }, { L"}", L"]", L"\"", L"'" }, { L"", L"", L"\\", L"\\" });
            TRY
                auto json = std::make_shared<Json>();
                JsonBuilder builder;
                builder.Deserialize(jsonStr, json);
                return json;
            CATCH
        }
    CATCH
    return nullptr;
}

std::vector<std::wstring> VPGEnumClassReader::GetAttribute(const std::wstring &str) const
{
    std::vector<std::wstring> result;
    TRY
        size_t pos = 0;
        GetNextCharPos(str, pos, true);

        if (!IsStartWith(str, attributePrefix, pos))
            return result;
        
        std::vector<std::wstring> tokens = SplitString(str.substr(pos), { attributePrefix });
        for (auto &token : tokens) {
            Trim(token);
            if (token.empty())
                continue;
            
            result.push_back(attributePrefix + token);
        }
    CATCH
    return result;
}

void VPGEnumClassReader::_AssignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::shared_ptr<VPGEnumClassProperty> property) const
{
    TRY
        size_t pos = 0;
        property->_Macro = _GetMacro(propertyCommand, pos);
        Trim(property->_Macro);
        
        std::wstring remainStr = property->_Macro.size() < propertyCommand.size() ? propertyCommand.substr(property->_Macro.size()) : L"";
        // if Macro is empty, then rollback to pos = 0
        if (property->_Macro.empty()) {
            property->_PropertyType = VPGEnumClassPropertyType::NA;
            property->_Command = remainStr;
            Trim(property->_Command);
            return;
        }

        // split macro
        pos = 0;
        if (IsStartWith(property->_Macro, L"MANAGER", pos)) {
            property->_PropertyType = VPGEnumClassPropertyType::Manager;

            property->_Type1 = _GetType(property->_Macro, pos);

            if (property->_Macro[pos] == L',')  {
                pos++;
                property->_PropertyName = _GetPropertyName(property->_Macro, pos);
            }
            if (property->_Macro[pos] == L',')  {
                pos++;
                property->_DefaultValue = _GetDefaultValue(property->_Macro, pos);
            }
        } else if (IsStartWith(property->_Macro, L"ACTION", pos)) {
            property->_PropertyType = VPGEnumClassPropertyType::Action;

            property->_PropertyName = _GetType(property->_Macro, pos);
            if (property->_Macro[pos] == L',')  {
                pos++;
                property->_Type1 = _GetPropertyName(property->_Macro, pos);
            }
        } else {
            property->_PropertyType = VPGEnumClassPropertyType::Property;

            if (IsStartWith(property->_Macro, L"VECTOR", pos))
                property->_GetSetType = VPGEnumClassGetSetType::Vector;
            else if (IsStartWith(property->_Macro, L"MAP", pos))
                property->_GetSetType = VPGEnumClassGetSetType::Map;
            else if (IsStartWith(property->_Macro, L"ORDERED_MAP", pos))
                property->_GetSetType = VPGEnumClassGetSetType::OrderedMap;
            else if (IsStartWith(property->_Macro, L"SET", pos))
                property->_GetSetType = VPGEnumClassGetSetType::Set;
            else
                property->_GetSetType = VPGEnumClassGetSetType::General;
            
            size_t posOfOpenQuote = Find(property->_Macro, L"(");
            if (posOfOpenQuote != std::wstring::npos)
                property->_IsObject = IsContain(property->_Macro.substr(0, posOfOpenQuote) , L"_SPTR");

            if (IsStartWith(property->_Macro, L"MAP", pos) || IsStartWith(property->_Macro, L"ORDERED_MAP", pos)) {
                property->_Type1 = _GetType(property->_Macro, pos);
                if (property->_Macro[pos] == L',')  {
                    pos++;
                    property->_Type2 = _GetPropertyName(property->_Macro, pos);
                }
                if (property->_Macro[pos] == L',')  {
                    pos++;
                    property->_PropertyName = _GetDefaultValue(property->_Macro, pos);
                }
            } else {
                property->_Type1 = _GetType(property->_Macro, pos);

                if (property->_Macro[pos] == L',')  {
                    pos++;
                    property->_PropertyName = _GetPropertyName(property->_Macro, pos);
                }
                if (property->_Macro[pos] == L',')  {
                    pos++;
                    property->_DefaultValue = _GetDefaultValue(property->_Macro, pos);
                }
            }
        }
        
        // split Remain
        std::vector<std::wstring> attributes = GetAttribute(remainStr);
        for (auto const &attribute : attributes) {
            std::vector<std::wstring> attributeTokes = SplitStringBySpace(attribute);
            std::wstring attributeToken = !attributeTokes.empty() ? attributeTokes[0] : L"";
            // Privilege
            if (IsEqual(attributeToken, attributePrefix + L"ReadOnly", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::ReadOnly;
            else if (IsEqual(attributeToken, attributePrefix + L"WriteOnly", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::WriteOnly;
            else if (IsEqual(attributeToken, attributePrefix + L"ReadWrite", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::ReadWrite;
            else if (IsEqual(attributeToken, attributePrefix + L"NoAccess", true))
                property->_AccessMode = VPGEnumClassPropertyAccessMode::NoAccess;
            else if (IsEqual(attributeToken, attributePrefix + L"Inherit", true))
                property->SetIsInherit(true);
            // Action
            else if (IsEqual(attributeToken, attributePrefix + L"NoHistory", true))
                property->SetIsNoHistory(true);
            else if (IsEqual(attributeToken, attributePrefix + L"ActionResult", true)) {
                auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"ActionResult");
                if (jsonAttributes == nullptr
                    || !((jsonAttributes->IsContainKey(L"Redo.Class") && !IsBlank(jsonAttributes->GetString(L"Redo.Class")))
                        || (jsonAttributes->IsContainKey(L"Undo.Class") && !IsBlank(jsonAttributes->GetString(L"Undo.Class")))))
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->_Name + L" has attribute @@ActionResult but missing Attribute \"Redo.Class\" or \"Undo.Class\"");
                if (jsonAttributes->IsContainKey(L"Redo.Class") && !IsBlank(jsonAttributes->GetString(L"Redo.Class")))
                    property->_ActionResultRedoClass = jsonAttributes->GetString(L"Redo.Class");
                if (jsonAttributes->IsContainKey(L"Undo.Class") && !IsBlank(jsonAttributes->GetString(L"Undo.Class")))
                    property->_ActionResultUndoClass = jsonAttributes->GetString(L"Undo.Class");
            }
            // Json
            else if (IsEqual(attributeToken, attributePrefix + L"NoJson", true))
                property->SetIsNoJson(true);
            // Command
            else if (IsEqual(attributeToken, attributePrefix + L"Command", true)) {
                std::wstring commandToken = attributePrefix + L"Command";
                commandToken = attribute.substr(commandToken.length());
                Trim(commandToken);
                property->SetCommand(commandToken);
            }
        }
    CATCH
}

std::wstring VPGEnumClassReader::_GetCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const
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
            size_t currentPos = pos;
            if (isClassCommand) {
                size_t nextNewLinePos = cppCode.find(L"\n", pos + 1);
                GetNextCharPos(cppCode, pos, false);
                if (nextNewLinePos != std::wstring::npos && pos != std::wstring::npos
                    && nextNewLinePos < pos && (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos))) {
                    result = L"";
                }
                pos = currentPos;
            }
            GetNextCharPos(cppCode, pos, false);
        }
        Trim(result);
        // need to back 1 char as used GetNextCharPos
        pos--;
    CATCH
    return result;
}

void VPGEnumClassReader::_ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass> enumClass) const
{
    TRY
        _EnumValue = -1;
        while (pos < cppCode.size()) {
            if (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
                _GetCommand(cppCode, false, pos);
                continue;
            }
            std::wstring name = _GetEnum(cppCode, pos);
            Trim(name);
            if (name.empty())
                break;
            
            auto property = std::make_shared<VPGEnumClassProperty>();
            property->_Enum = name;
            GetNextCharPos(cppCode, pos, false);
            if (cppCode[pos] == L'=') {
                std::wstring enumValueStr = L"";
                for (; pos < cppCode.size(); pos++) {
                    if (std::iswdigit(cppCode[pos]))
                        enumValueStr += cppCode[pos];
                    else {
                        if (!enumValueStr.empty())
                            break;
                    }
                }
                if (!enumValueStr.empty()) {
                    _EnumValue = std::stod(enumValueStr);
                    _EnumValue--;
                }
                pos--;
                GetNextCharPos(cppCode, pos, false);
            }
            _EnumValue++;
            property->_EnumValue = _EnumValue;
            if (cppCode[pos] == L',')
                GetNextCharPos(cppCode, pos, false);
            if (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
                _AssignEnumClassProperty(enumClass.get(), _GetCommand(cppCode, false, pos), property);
                GetNextCharPos(cppCode, pos, false);
            }
            if (cppCode[pos] == L',')
                GetNextCharPos(cppCode, pos, false);

            enumClass->_Properties.push_back(property);

            if (cppCode[pos] == L'}') {
                pos--;
                return;
            }
        }
    CATCH
}

bool VPGEnumClassReader::_ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const
{
    TRY
        if (!IsStartWith(cppCode, L"enum", pos))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"enum missing."));
            
        pos += 4; // length of "enum"
        GetNextCharPos(cppCode, pos, false);
        if (IsStartWith(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            GetNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"Class Name missing."));

        enumClass->_Name = _GetEnum(cppCode, pos);
        GetNextCharPos(cppCode, pos, false);

        if (IsStartWith(cppCode, L"//", pos)) {
            enumClass->_Command = _GetCommand(cppCode, false, pos);
            GetNextCharPos(cppCode, pos, false);
        } else if (IsStartWith(cppCode, L"/*", pos)) {
            enumClass->_Command =_GetCommand(cppCode, false, pos);
            GetNextCharPos(cppCode, pos, false);
        }
        if (!enumClass->_Command.empty()) {
            std::vector<std::wstring> attributes = GetAttribute(enumClass->_Command);
            std::wstring command = L"";
            for (auto const &attribute : attributes) {
                if (IsStartWith(attribute, attributePrefix + L"Form", 0, true)) {
                    enumClass->_Type = VPGEnumClassType::Form;
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"ActionArgument", 0, true)) {
                    enumClass->_Type = VPGEnumClassType::ActionArgument;
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Result", 0, true)) {
                    enumClass->_Type = VPGEnumClassType::Result;
                    command = L"";
                }  else if (IsStartWith(attribute, attributePrefix + L"Inherit", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Inherit");
                    assert(jsonAttributes != nullptr);
                    std::wstring className = jsonAttributes->GetString(L"Class");
                    if (IsBlank(className))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->_Name + L" has attribute @@Inherit but missing Attribute \"Class\"");
                    enumClass->_InheritClass = className;
                
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Log", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Log");
                    if (jsonAttributes != nullptr)
                        enumClass->_IsLogConfigIndependent = jsonAttributes->GetBool(L"IsIndependent");
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Action", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Action");
                    if (jsonAttributes != nullptr)
                        enumClass->_IsActionManagerIndependent = jsonAttributes->GetBool(L"IsIndependent");
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Thread", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Thread");
                    if (jsonAttributes != nullptr)
                        enumClass->_IsThreadManagerIndependent = jsonAttributes->GetBool(L"IsIndependent");
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Json", 0, true)) {
                    enumClass->_IsJson = true;
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Json");
                    if (jsonAttributes != nullptr) {
                        for (auto const &key : jsonAttributes->GetKeys())
                            enumClass->InsertJsonAttributesAtKey(key, jsonAttributes->GetString(key));
                    }
                    command = L"";
                } else if (IsStartWith(attribute, attributePrefix + L"Command", 0, true)) {
                    std::wstring commandToken = attributePrefix + L"Command";
                    commandToken = attribute.substr(commandToken.length());
                    Trim(commandToken);
                    command = commandToken;
                }
                
                enumClass->_Command = command;
            }
        }
        if (cppCode[pos] == L';') {
            return false;
        } else if (cppCode[pos] != L'{')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"{ missing."));
        GetNextCharPos(cppCode, pos, false);

        _ParseProperties(cppCode, pos, enumClass);
        GetNextCharPos(cppCode, pos, false);

        if (cppCode[pos] != L'}')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"} missing."));
    CATCH
    return true;
}

std::wstring VPGEnumClassReader::GetCppCodeLine(const std::wstring &str, size_t &pos, bool fromCurrentPos) const
{
    if (!fromCurrentPos)
        pos++;
    size_t startPos = pos;
    TRY
        pos = Find(str, L"\n", pos);
        std::wstring tailingSubstr = GetTailingSubstring(str.substr(startPos, pos - startPos + 1), 3);
        if (IsEndWith(tailingSubstr, L"\\\r\n") || IsEndWith(tailingSubstr, L"\\\n")) {
            GetCppCodeLine(str, pos, false);
        }
        if (pos == std::wstring::npos)
            pos = str.length();
    CATCH
    return str.substr(startPos, pos - startPos + 1);
}

void VPGEnumClassReader::Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        std::unique_lock lock(_mutex);
        size_t pos = 0;
        std::wstring currentCommand = L"";
        std::wstring currentNamespace = L"";
        size_t namespaceEndPos = 0;
        while (pos < cppCode.size()) {
            if (namespaceEndPos > 0 && pos > namespaceEndPos)
                currentNamespace = L"";

            if (IsStartWith(cppCode, L"namespace", pos)) {
                GetNextString(cppCode, pos, { L" ", L"\t", L"\r", L"\n" });
                pos++;
                currentNamespace = GetNextString(cppCode, pos, { L" ", L"\t", L"\r", L"\n", L";" });
                pos++;
                size_t openQuotePos = Find(cppCode, L"{", pos);
                size_t semiPos = Find(cppCode, L";", pos);
                if (openQuotePos == std::wstring::npos && semiPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, openQuotePos, L"Namespace " + currentNamespace + L" missing { or ;"));
                if (semiPos == std::wstring::npos || openQuotePos < semiPos) {
                    pos = openQuotePos;
                    namespaceEndPos = pos;
                    GetNextQuotedString(cppCode, namespaceEndPos, { L" ", L"\t", L"\r", L"\n", L";" }, { L"{", L"\"", L"//", L"/*" }, { L"}", L"\"", L"\n", L"*/" }, { L"", L"\\", L"", L"" });
                } else {
                    pos = semiPos;
                    currentNamespace = L"";
                    namespaceEndPos = 0;
                }
                currentCommand = L"";
            } else if (IsStartWith(cppCode, L"//", pos) || IsStartWith(cppCode, L"/*", pos)) {
                std::wstring tmpCmd = _GetCommand(cppCode, true, pos);
                Trim(tmpCmd);
                if (!currentCommand.empty())
                    currentCommand += L"\r\n";
                currentCommand += tmpCmd;
            } else if (cppCode[pos] == L'#') {
                GetCppCodeLine(cppCode, pos, true);
                currentCommand = L"";
            } else if (IsStartWith(cppCode, L"enum ", pos)) {
                auto enumClass = std::make_shared<VPGEnumClass>();
                enumClass->SetNamespace(currentNamespace);
                enumClass->SetCommand(currentCommand);
                bool isFullEnumClass = _ParseClass(cppCode, pos, enumClass);
                if (isFullEnumClass)
                    results.push_back(enumClass);
                currentCommand = L"";
            }
            GetNextCharPos(cppCode, pos, false);
        }
    CATCH
}