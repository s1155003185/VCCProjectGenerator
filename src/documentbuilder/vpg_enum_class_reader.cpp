#include "vpg_enum_class_reader.hpp"

#include <assert.h>
#include <math.h>
#include <string>
#include <mutex>
#include <vector>

#include "exception_macro.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "set_helper.hpp"
#include "string_helper.hpp"
#include "vpg_enum_class.hpp"

const std::wstring attributePrefix = L"@@";
std::mutex _mutex;

VPGEnumClassReader::VPGEnumClassReader(const std::set<std::wstring> &classMacroList) 
{
    this->_ClassMacroList.insert(classMacroList.begin(), classMacroList.end());
}

std::wstring VPGEnumClassReader::_GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const
{
        size_t row = 0, column = 0;
        vcc::GetCharacterRowAndColumn(str, pos, row, column);
        
        size_t lengthOfPos = std::min(pos, (size_t)100);
        size_t lengthOfSub = std::min(str.length() - pos, (size_t)100);

        std::wstring preString = str.substr(pos - lengthOfPos, lengthOfPos);
        std::wstring subString = str.substr(pos + 1, lengthOfSub);
        return L"Error at position " + std::to_wstring(pos + 1) 
            + L", row " + std::to_wstring(row) 
            + L", column " + std::to_wstring(column) + L": " + preString + L"<<" + std::wstring(1, str[pos]) + L">>" + subString + L"\r\n "
            + L" Error message: " + msg;
}

size_t VPGEnumClassReader::IsCommand(const std::wstring &cppCode, const size_t &pos) const
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

std::wstring VPGEnumClassReader::_GetEnum(const std::wstring &propertyStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        vcc::GetNextCharPos(propertyStr, pos, true);
        while (pos < propertyStr.size())
        {
            if (!std::iswalpha(propertyStr[pos]) && !std::iswdigit(propertyStr[pos]) && propertyStr[pos] != L'_') {
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

        vcc::GetNextCharPos(propertyCommand, pos, true);
        bool hasMacroPrefix = false;
        for (auto const &str : this->_ClassMacroList) {
            if (vcc::IsStartWith(propertyCommand, str + L"(", pos)) {
                hasMacroPrefix = true;
                break;
            }
        }
        if (!hasMacroPrefix) {
            pos = 0;
            return result;
        }

        size_t posOfQuote = vcc::Find(propertyCommand, L"(", pos);
        result = propertyCommand.substr(pos, posOfQuote - pos);
        pos = posOfQuote;
        result += vcc::GetNextQuotedString(propertyCommand, pos, { L")", L" ", L"\t", L"\r", L"\n" }, { L"\"", L"'", L"{", L"[", L"(", L"/*", L"//" }, { L"\"", L"'", L"}", L"]", L")", L"*/", L"\n" }, { L"\\", L"\\", L"", L"", L"", L"", L"" }, { L"\"", L"//", L"/*" });
        vcc::Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetType(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        pos = vcc::Find(macroStr, L"(");
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro ( missing");
        pos++;
        size_t endPos = vcc::Find(macroStr, L",");
        if (endPos == std::wstring::npos) {
            endPos = vcc::Find(macroStr, L")");
            if (endPos == std::wstring::npos)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetPropertyName(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = vcc::Find(macroStr, L",", pos);
        if (endPos == std::wstring::npos) {
            endPos = macroStr.find_last_of(L")");
            if (endPos == std::wstring::npos) 
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::Trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_GetDefaultValue(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = macroStr.find_last_of(L")");
        if (endPos == std::wstring::npos) 
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro ) missing");

        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::Trim(result);
    CATCH
    return result;
}

std::shared_ptr<vcc::Json> VPGEnumClassReader::GetJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const
{
    TRY
        size_t pos = vcc::Find(command, attributeName, 0, true);
        if (pos == std::wstring::npos)
            return nullptr;
        pos += attributeName.length();
        vcc::GetNextCharPos(command, pos, true);
        if (command[pos] == L'{') {
            std::wstring jsonStr = vcc::GetNextQuotedString(command, pos, { L" ", L"\t", L"\n" }, { L"{", L"[", L"\"", L"'" }, { L"}", L"]", L"\"", L"'" }, { L"", L"", L"\\", L"\\" }, { L"\"" });
            TRY
                auto json = std::make_shared<vcc::Json>();
                vcc::JsonBuilder builder;
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
        vcc::GetNextCharPos(str, pos, true);

        if (!vcc::IsStartWith(str, attributePrefix, pos))
            return result;
        
        std::vector<std::wstring> tokens = vcc::SplitString(str.substr(pos), { attributePrefix });
        for (auto &token : tokens) {
            vcc::Trim(token);
            if (token.empty())
                continue;
            
            result.push_back(attributePrefix + token);
        }
    CATCH
    return result;
}

void VPGEnumClassReader::_AssignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::vector<std::shared_ptr<VPGEnumClassAttribute>> &properties) const
{
    TRY
        auto property = std::make_shared<VPGEnumClassAttribute>();
        std::vector<std::wstring> macroList;
        // 1. Get all macro from command
        size_t pos = 0;
        while (true) {
            size_t lastPos = pos;
            std::wstring macro = _GetMacro(propertyCommand, lastPos);
            vcc::Trim(macro);
            if (lastPos != 0) {
                pos = lastPos;
                pos++;
            }
            if (macro.empty())
                break;
            macroList.push_back(macro);
        }
        std::wstring remainStr = pos < propertyCommand.size() ? propertyCommand.substr(pos) : L"";
        vcc::Trim(remainStr);
        // if Macro is empty, then rollback to pos = 0
        if (macroList.empty()) {
            property->SetPropertyType(VPGEnumClassAttributeType::NA);
            property->SetCommand(remainStr);
            properties.push_back(property);
            return;
        }
        // 2. Create a property and assign all property, then clone and assign macro
        // split Remain
        std::vector<std::wstring> attributes = GetAttribute(remainStr);
        for (auto const &attribute : attributes) {
            std::vector<std::wstring> attributeTokes = vcc::SplitStringBySpace(attribute);
            std::wstring attributeToken = !attributeTokes.empty() ? attributeTokes[0] : L"";
            // Privilege
            if (vcc::IsEqual(attributeToken, attributePrefix + L"ReadOnly", true))
                property->SetAccessMode(VPGEnumClassAttributeAccessMode::ReadOnly);
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"WriteOnly", true))
                property->SetAccessMode(VPGEnumClassAttributeAccessMode::WriteOnly);
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"ReadWrite", true))
                property->SetAccessMode(VPGEnumClassAttributeAccessMode::ReadWrite);
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"NoAccess", true))
                property->SetAccessMode(VPGEnumClassAttributeAccessMode::NoAccess);
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"Inherit", true))
                property->SetIsInherit(true);
            // Property
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"Initialize", true)) {
                auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Initialize");
                if (jsonAttributes != nullptr && jsonAttributes->IsContainKey(L"Properties")) {
                    for (auto const &element : jsonAttributes->GetArray(L"Properties"))
                        property->InsertInitializeProperties(element->GetArrayElementString());
                }
            }
            // Action
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"NoHistory", true))
                property->SetIsNoHistory(true);
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"ActionResult", true)) {
                auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"ActionResult");
                if (jsonAttributes == nullptr
                    || !((jsonAttributes->IsContainKey(L"Redo.Class") && !vcc::IsBlank(jsonAttributes->GetString(L"Redo.Class")))
                        || (jsonAttributes->IsContainKey(L"Undo.Class") && !vcc::IsBlank(jsonAttributes->GetString(L"Undo.Class")))))
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->GetName() + L" has attribute @@ActionResult but missing Attribute \"Redo.Class\" or \"Undo.Class\"");
                if (jsonAttributes->IsContainKey(L"Redo.Class") && !vcc::IsBlank(jsonAttributes->GetString(L"Redo.Class")))
                    property->SetActionResultRedoClass(jsonAttributes->GetString(L"Redo.Class"));
                if (jsonAttributes->IsContainKey(L"Undo.Class") && !vcc::IsBlank(jsonAttributes->GetString(L"Undo.Class")))
                    property->SetActionResultUndoClass(jsonAttributes->GetString(L"Undo.Class"));
            }
            // Json
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"NoJson", true))
                property->SetIsNoJson(true);
            // Command
            else if (vcc::IsEqual(attributeToken, attributePrefix + L"Command", true)) {
                std::wstring commandToken = attributePrefix + L"Command";
                commandToken = attribute.substr(commandToken.length());
                vcc::Trim(commandToken);
                property->SetCommand(commandToken);
            }
        }

        for (auto macro : macroList) {
            auto currentProperty = std::dynamic_pointer_cast<VPGEnumClassAttribute>(property->Clone());
            // split macro
            currentProperty->SetMacro(macro);

            // virtual
            if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETCUSTOM("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Getcustom);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SETCUSTOM("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Setcustom);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETCUSTOM_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::GetcustomSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SETCUSTOM_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::SetcustomSptr);

            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETSET("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Getset);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETSET_VALIDATE("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::GetsetValidate);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETSET_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::GetsetSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETSET_SPTR_NULL("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::GetsetSptrNull);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"GETSET_VALIDATE_SPTR_NULL("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::GetsetValidateSptrNull);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"VECTOR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Vector);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"VECTOR_VALIDATE("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::VectorValidate);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"VECTOR_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::VectorSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"VECTOR_VALIDATE_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::VectorValidateSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SET("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Set);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SET_VALIDATE("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::SetValidate);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SET_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::SetSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"SET_VALIDATE_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::SetValidateSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MAP("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Map);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MAP_VALIDATE("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::MapValidate);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MAP_SPTR_R("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::MapSptrR);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MAP_VALIDATE_SPTR_R("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::MapValidateSptrR);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ORDERED_MAP("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::OrderedMap);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ORDERED_MAP_VALIDATE("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::OrderedMapValidate);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ORDERED_MAP_SPTR_R("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::OrderedMapSptrR);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ORDERED_MAP_VALIDATE_SPTR_R("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::OrderedMapValidateSptrR);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MANAGER_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::ManagerSptr);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MANAGER_SPTR_NULL("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::ManagerSptrNull);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"MANAGER_SPTR_PARENT("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::ManagerSptrParent);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ACTION("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::Action);
            else if (vcc::IsStartWith(currentProperty->GetMacro(), L"ACTION_WITH_ARG_SPTR("))
                currentProperty->SetMacroType(VPGEnumClassMacroType::ActionWithArgSptr);

            pos = 0;
            if (currentProperty->GetIsManager()) {
                currentProperty->SetPropertyType(VPGEnumClassAttributeType::Manager);
    
                currentProperty->SetType1(_GetType(currentProperty->GetMacro(), pos));
    
                if (currentProperty->GetMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->SetPropertyName(_GetPropertyName(currentProperty->GetMacro(), pos));
                }
                if (currentProperty->GetMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->SetDefaultValue(_GetDefaultValue(currentProperty->GetMacro(), pos));
                }
            } else if (currentProperty->GetIsAction()) {
                currentProperty->SetPropertyType(VPGEnumClassAttributeType::Action);
    
                currentProperty->SetPropertyName(_GetType(currentProperty->GetMacro(), pos));
                if (currentProperty->GetMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->SetType1(_GetPropertyName(currentProperty->GetMacro(), pos));
                }
            } else {
                currentProperty->SetPropertyType(VPGEnumClassAttributeType::Property);
    
                if (currentProperty->GetIsMap() || currentProperty->GetIsOrderedMap()) {
                    currentProperty->SetType1(_GetType(currentProperty->GetMacro(), pos));
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetType2(_GetPropertyName(currentProperty->GetMacro(), pos));
                    }
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetPropertyName(_GetPropertyName(currentProperty->GetMacro(), pos));
                    }
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetValidate(_GetDefaultValue(currentProperty->GetMacro(), pos));
                    }
                } else if (currentProperty->GetMacroType() == VPGEnumClassMacroType::Setcustom || currentProperty->GetMacroType() == VPGEnumClassMacroType::SetcustomSptr) {
                    currentProperty->SetPropertyName(_GetType(currentProperty->GetMacro(), pos));
    
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetType1(_GetPropertyName(currentProperty->GetMacro(), pos));
                    }
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetDefaultValue(_GetDefaultValue(currentProperty->GetMacro(), pos));
                    }
                } else {
                    currentProperty->SetType1(_GetType(currentProperty->GetMacro(), pos));
    
                    if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetPropertyName(_GetPropertyName(currentProperty->GetMacro(), pos));
                    }
                    if (currentProperty->GetMacroType() == VPGEnumClassMacroType::GetsetValidate) {
                        if (currentProperty->GetMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->SetDefaultValue(_GetPropertyName(currentProperty->GetMacro(), pos));
                        }
                        if (currentProperty->GetMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->SetValidate(_GetDefaultValue(currentProperty->GetMacro(), pos));
                        }
                    } else if (currentProperty->GetIsHavingValidate() ) {
                        if (currentProperty->GetMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->SetValidate(_GetDefaultValue(currentProperty->GetMacro(), pos));
                        }
                    } else if (currentProperty->GetMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->SetDefaultValue(_GetDefaultValue(currentProperty->GetMacro(), pos));
                    }
                }
            }
            properties.push_back(currentProperty);
        }
    CATCH
}

std::wstring VPGEnumClassReader::_GetCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        vcc::GetNextCharPos(cppCode, pos, true);
        while (vcc::IsStartWith(cppCode, L"//", pos) || vcc::IsStartWith(cppCode, L"/*", pos)) {
            std::wstring tmpCmd = vcc::GetNextQuotedString(cppCode, pos, { L";", L"{", L"\n", L" ", L"/*", L"//" }, { L"/*", L"//"}, {L"*/", L"\n"}, { L"", L"" }, { L"/*", L"//"});
            vcc::Trim(tmpCmd);
            if (!result.empty())
                result += L"\r\n";
            if (vcc::IsStartWith(tmpCmd, L"//")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 2); // only minus prefix as already vcc::Trim
            } else if (vcc::IsStartWith(tmpCmd, L"/*")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 4);
            }
            vcc::Trim(tmpCmd);
            result += tmpCmd;
            size_t currentPos = pos;
            if (isClassCommand) {
                size_t nextNewLinePos = cppCode.find(L"\n", pos + 1);
                vcc::GetNextCharPos(cppCode, pos, false);
                if (nextNewLinePos != std::wstring::npos && pos != std::wstring::npos
                    && nextNewLinePos < pos && (vcc::IsStartWith(cppCode, L"//", pos) || vcc::IsStartWith(cppCode, L"/*", pos))) {
                    result = L"";
                }
                pos = currentPos;
            }
            vcc::GetNextCharPos(cppCode, pos, false);
        }
        vcc::Trim(result);
        // need to back 1 char as used vcc::GetNextCharPos
        pos--;
    CATCH
    return result;
}

void VPGEnumClassReader::_ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass> enumClass) const
{
    TRY
        _EnumValue = -1;
        while (pos < cppCode.size()) {
            if (vcc::IsStartWith(cppCode, L"//", pos) || vcc::IsStartWith(cppCode, L"/*", pos)) {
                _GetCommand(cppCode, false, pos);
                continue;
            }
            std::wstring enumName = _GetEnum(cppCode, pos);
            vcc::Trim(enumName);
            if (enumName.empty())
                break;
            
            vcc::GetNextCharPos(cppCode, pos, false);
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
                vcc::GetNextCharPos(cppCode, pos, false);
            }
            _EnumValue++;

            if (cppCode[pos] == L',')
                vcc::GetNextCharPos(cppCode, pos, false);
            
            std::vector<std::shared_ptr<VPGEnumClassAttribute>> properties;
            if (vcc::IsStartWith(cppCode, L"//", pos) || vcc::IsStartWith(cppCode, L"/*", pos)) {
                _AssignEnumClassProperty(enumClass.get(), _GetCommand(cppCode, false, pos), properties);
                vcc::GetNextCharPos(cppCode, pos, false);
            }
            if (cppCode[pos] == L',')
                vcc::GetNextCharPos(cppCode, pos, false);

            if (properties.empty())
                properties.push_back(std::make_shared<VPGEnumClassAttribute>());
            
            for (auto property : properties) {
                property->SetEnum(enumName);
                property->SetEnumValue(_EnumValue);
            }
            enumClass->GetProperties().insert(enumClass->GetProperties().end(), properties.begin(), properties.end());
            
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
        if (!vcc::IsStartWith(cppCode, L"enum", pos))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"enum missing."));
            
        pos += 4; // length of "enum"
        vcc::GetNextCharPos(cppCode, pos, false);
        if (vcc::IsStartWith(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            vcc::GetNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"Class Name missing."));

        enumClass->SetName(_GetEnum(cppCode, pos));
        vcc::GetNextCharPos(cppCode, pos, false);

        if (vcc::IsStartWith(cppCode, L"//", pos)) {
            enumClass->SetCommand(_GetCommand(cppCode, false, pos));
            vcc::GetNextCharPos(cppCode, pos, false);
        } else if (vcc::IsStartWith(cppCode, L"/*", pos)) {
            enumClass->SetCommand(_GetCommand(cppCode, false, pos));
            vcc::GetNextCharPos(cppCode, pos, false);
        }
        if (!enumClass->GetCommand().empty()) {
            std::vector<std::wstring> attributes = GetAttribute(enumClass->GetCommand());
            std::wstring command = L"";
            for (auto const &attribute : attributes) {
                if (vcc::IsStartWith(attribute, attributePrefix + L"Form", 0, true)) {
                    enumClass->SetType(VPGEnumClassType::Form);
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"ActionArgument", 0, true)) {
                    enumClass->SetType(VPGEnumClassType::ActionArgument);
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Result", 0, true)) {
                    enumClass->SetType(VPGEnumClassType::Result);
                    command = L"";
                }  else if (vcc::IsStartWith(attribute, attributePrefix + L"Inherit", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Inherit");
                    assert(jsonAttributes != nullptr);
                    std::wstring className = jsonAttributes->GetString(L"Class");
                    if (vcc::IsBlank(className))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->GetName() + L" has attribute @@Inherit but missing Attribute \"Class\"");
                    enumClass->SetInheritClass(className);
                
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Log", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Log");
                    if (jsonAttributes != nullptr)
                        enumClass->SetIsLogConfigIndependent(jsonAttributes->GetBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Action", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Action");
                    if (jsonAttributes != nullptr)
                        enumClass->SetIsActionManagerIndependent(jsonAttributes->GetBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Thread", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Thread");
                    if (jsonAttributes != nullptr)
                        enumClass->SetIsThreadManagerIndependent(jsonAttributes->GetBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Json", 0, true)) {
                    enumClass->SetIsJson(true);
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Json");
                    if (jsonAttributes != nullptr) {
                        for (auto const &key : jsonAttributes->GetKeys())
                            enumClass->InsertJsonAttributesAtKey(key, jsonAttributes->GetString(key));
                    }
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Include", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Include");
                    if (jsonAttributes != nullptr) {
                        if (jsonAttributes->IsContainKey(L"SystemFiles"))
                            for (auto const &element : jsonAttributes->GetArray(L"SystemFiles"))
                                enumClass->InsertIncludeSystemFiles(element->GetArrayElementString());
                        if (jsonAttributes->IsContainKey(L"CustomFiles"))
                            for (auto const &element : jsonAttributes->GetArray(L"CustomFiles"))
                                enumClass->InsertIncludeCustomFiles(element->GetArrayElementString());
                    }
                    command = L"";
                }  else if (vcc::IsStartWith(attribute, attributePrefix + L"Private", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Private");
                    if (jsonAttributes != nullptr && jsonAttributes->IsContainKey(L"Properties")) {
                        auto element = jsonAttributes->GetObject(L"Properties");
                        if (element != nullptr) {
                            for (auto const &key : element->GetKeys())
                                enumClass->InsertPrivatePropertiesAtKey(key, element->GetString(key));
                        }
                    }
                    command = L"";
                }  else if (vcc::IsStartWith(attribute, attributePrefix + L"Protected", 0, true)) {
                    auto jsonAttributes = GetJsonAttributes(attribute, attributePrefix + L"Protected");
                    if (jsonAttributes != nullptr && jsonAttributes->IsContainKey(L"Properties")) {
                        auto element = jsonAttributes->GetObject(L"Properties");
                        if (element != nullptr) {
                            for (auto const &key : element->GetKeys())
                                enumClass->InsertProtectedPropertiesAtKey(key, element->GetString(key));
                        }
                    }
                    command = L"";
                } else if (vcc::IsStartWith(attribute, attributePrefix + L"Command", 0, true)) {
                    std::wstring commandToken = attributePrefix + L"Command";
                    commandToken = attribute.substr(commandToken.length());
                    vcc::Trim(commandToken);
                    command = commandToken;
                }
                
                enumClass->SetCommand(command);
            }
        }
        if (cppCode[pos] == L';') {
            return false;
        } else if (cppCode[pos] != L'{')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(cppCode, pos, L"{ missing."));
        vcc::GetNextCharPos(cppCode, pos, false);

        _ParseProperties(cppCode, pos, enumClass);
        vcc::GetNextCharPos(cppCode, pos, false);

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
        pos = vcc::Find(str, L"\n", pos);
        std::wstring tailingSubstr = vcc::GetTailingSubstring(str.substr(startPos, pos - startPos + 1), 3);
        if (vcc::IsEndWith(tailingSubstr, L"\\\r\n") || vcc::IsEndWith(tailingSubstr, L"\\\n")) {
            GetCppCodeLine(str, pos, false);
        }
        if (pos == std::wstring::npos)
            pos = str.length();
    CATCH
    return str.substr(startPos, pos - startPos + 1);
}

void VPGEnumClassReader::ParseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        size_t pos = 0;
        std::wstring currentCommand = L"";
        bool isNamespaceTriggered = false;
        vcc::GetNextCharPos(cppCode, pos, true);
        while (pos < cppCode.size()) {
            size_t commandIndex = IsCommand(cppCode, pos);
            if (commandIndex != std::wstring::npos) {
                std::wstring tmpCmd = _GetCommand(cppCode, true, pos);
                vcc::Trim(tmpCmd);
                if (!currentCommand.empty())
                    currentCommand += L"\r\n";
                currentCommand += tmpCmd;
            } else {
                size_t previousPos = pos;
                std::wstring nextToken = vcc::GetNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                if (!vcc::IsBlank(nextToken)) {
                    if (isNamespaceTriggered) {
                        // namespace name
                        vcc::Trim(nextToken);
                        pos++;
                        std::wstring quoteStr = vcc::GetNextQuotedString(cppCode, pos, { L";" } );
                        vcc::Trim(quoteStr);
                        if (!quoteStr.empty()) {
                            std::vector<std::shared_ptr<VPGEnumClass>> tmpClassList;
                            ParseCustom(quoteStr.substr(1, quoteStr.length() - 2), nextToken, tmpClassList);
                            if (!currentNamespace.empty()) {
                                for (auto &tmpClass : tmpClassList) {
                                    tmpClass->SetName(currentNamespace + L"::" + tmpClass->GetName());
                                    results.push_back(tmpClass);
                                }
                            } else
                                results.insert(results.end(), tmpClassList.begin(), tmpClassList.end());
                        }
                        isNamespaceTriggered = false;
                    } else {
                        if (nextToken == L"namespace") {
                            isNamespaceTriggered = true;
                        } else if (nextToken[0] == L'#') {
                            pos = previousPos;
                            GetCppCodeLine(cppCode, pos, true);
                        } else if (nextToken == L"enum") {
                            pos = previousPos;
                            auto enumClass = std::make_shared<VPGEnumClass>();
                            enumClass->SetCommand(currentCommand);
                            bool isFullEnumClass = _ParseClass(cppCode, pos, enumClass);
                            if (!currentNamespace.empty())
                                enumClass->SetName(currentNamespace + L"::" + enumClass->GetName());
                            if (isFullEnumClass)
                                results.push_back(enumClass);
                        }
                    }
                    currentCommand = L"";
                }
            }
            vcc::GetNextCharPos(cppCode, pos, false);
        }
    CATCH
}

void VPGEnumClassReader::Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        std::unique_lock lock(_mutex);
        ParseCustom(cppCode, L"", results);
    CATCH
}