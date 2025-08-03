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

std::wstring VPGEnumClassReader::_getErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const
{
        size_t row = 0, column = 0;
        vcc::getCharacterRowAndColumn(str, pos, row, column);
        
        size_t lengthOfPos = std::min(pos, (size_t)100);
        size_t lengthOfSub = std::min(str.length() - pos, (size_t)100);

        std::wstring preString = str.substr(pos - lengthOfPos, lengthOfPos);
        std::wstring subString = str.substr(pos + 1, lengthOfSub);
        return L"Error at position " + std::to_wstring(pos + 1) 
            + L", row " + std::to_wstring(row) 
            + L", column " + std::to_wstring(column) + L": " + preString + L"<<" + std::wstring(1, str[pos]) + L">>" + subString + L"\r\n "
            + L" Error message: " + msg;
}

size_t VPGEnumClassReader::isCommand(const std::wstring &cppCode, const size_t &pos) const
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

std::wstring VPGEnumClassReader::_getEnum(const std::wstring &propertyStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        vcc::getNextCharPos(propertyStr, pos, true);
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

std::wstring VPGEnumClassReader::_getMacro(const std::wstring &propertyCommand, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        if (this->_ClassMacroList.empty())
            return result;

        vcc::getNextCharPos(propertyCommand, pos, true);
        bool hasMacroPrefix = false;
        for (auto const &str : this->_ClassMacroList) {
            if (vcc::isStartWith(propertyCommand, str + L"(", pos)) {
                hasMacroPrefix = true;
                break;
            }
        }
        if (!hasMacroPrefix) {
            pos = 0;
            return result;
        }

        size_t posOfQuote = vcc::find(propertyCommand, L"(", pos);
        result = propertyCommand.substr(pos, posOfQuote - pos);
        pos = posOfQuote;
        result += vcc::getNextQuotedString(propertyCommand, pos, { L")", L" ", L"\t", L"\r", L"\n" }, { L"\"", L"'", L"{", L"[", L"(", L"/*", L"//" }, { L"\"", L"'", L"}", L"]", L")", L"*/", L"\n" }, { L"\\", L"\\", L"", L"", L"", L"", L"" }, { L"\"", L"//", L"/*" });
        vcc::trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_getType(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        pos = vcc::find(macroStr, L"(");
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro ( missing");
        pos++;
        size_t endPos = vcc::find(macroStr, L",");
        if (endPos == std::wstring::npos) {
            endPos = vcc::find(macroStr, L")");
            if (endPos == std::wstring::npos)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_getPropertyName(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = vcc::find(macroStr, L",", pos);
        if (endPos == std::wstring::npos) {
            endPos = macroStr.find_last_of(L")");
            if (endPos == std::wstring::npos) 
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro , or ) missing");
        }
        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::trim(result);
    CATCH
    return result;
}

std::wstring VPGEnumClassReader::_getDefaultValue(const std::wstring &macroStr, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        size_t endPos = macroStr.find_last_of(L")");
        if (endPos == std::wstring::npos) 
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, macroStr + L": Macro ) missing");

        result = macroStr.substr(pos, endPos - pos);
        pos = endPos;
        vcc::trim(result);
    CATCH
    return result;
}

std::shared_ptr<vcc::Json> VPGEnumClassReader::getJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const
{
    TRY
        size_t pos = vcc::find(command, attributeName, 0, true);
        if (pos == std::wstring::npos)
            return nullptr;
        pos += attributeName.length();
        vcc::getNextCharPos(command, pos, true);
        if (command[pos] == L'{') {
            std::wstring jsonStr = vcc::getNextQuotedString(command, pos, { L" ", L"\t", L"\n" }, { L"{", L"[", L"\"", L"'" }, { L"}", L"]", L"\"", L"'" }, { L"", L"", L"\\", L"\\" }, { L"\"" });
            TRY
                auto json = std::make_shared<vcc::Json>();
                vcc::JsonBuilder builder;
                builder.deserialize(jsonStr, json);
                return json;
            CATCH
        }
    CATCH
    return nullptr;
}

std::vector<std::wstring> VPGEnumClassReader::getAttribute(const std::wstring &str) const
{
    std::vector<std::wstring> result;
    TRY
        size_t pos = 0;
        vcc::getNextCharPos(str, pos, true);

        if (!vcc::isStartWith(str, attributePrefix, pos))
            return result;
        
        std::vector<std::wstring> tokens = vcc::splitString(str.substr(pos), { attributePrefix });
        for (auto &token : tokens) {
            vcc::trim(token);
            if (token.empty())
                continue;
            
            result.push_back(attributePrefix + token);
        }
    CATCH
    return result;
}

void VPGEnumClassReader::_assignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::vector<std::shared_ptr<VPGEnumClassAttribute>> &properties) const
{
    TRY
        auto property = std::make_shared<VPGEnumClassAttribute>();
        std::vector<std::wstring> macroList;
        // 1. Get all macro from command
        size_t pos = 0;
        while (true) {
            size_t lastPos = pos;
            std::wstring macro = _getMacro(propertyCommand, lastPos);
            vcc::trim(macro);
            if (lastPos != 0) {
                pos = lastPos;
                pos++;
            }
            if (macro.empty())
                break;
            macroList.push_back(macro);
        }
        std::wstring remainStr = pos < propertyCommand.size() ? propertyCommand.substr(pos) : L"";
        vcc::trim(remainStr);
        // if Macro is empty, then rollback to pos = 0
        if (macroList.empty()) {
            property->setPropertyType(VPGEnumClassAttributeType::NA);
            property->setCommand(remainStr);
            properties.push_back(property);
            return;
        }
        // 2. Create a property and assign all property, then clone and assign macro
        // split Remain
        std::vector<std::wstring> attributes = getAttribute(remainStr);
        for (auto const &attribute : attributes) {
            std::vector<std::wstring> attributeTokes = vcc::splitStringBySpace(attribute);
            std::wstring attributeToken = !attributeTokes.empty() ? attributeTokes[0] : L"";
            // Privilege
            if (vcc::isEqual(attributeToken, attributePrefix + L"ReadOnly", true))
                property->setAccessMode(VPGEnumClassAttributeAccessMode::ReadOnly);
            else if (vcc::isEqual(attributeToken, attributePrefix + L"WriteOnly", true))
                property->setAccessMode(VPGEnumClassAttributeAccessMode::WriteOnly);
            else if (vcc::isEqual(attributeToken, attributePrefix + L"ReadWrite", true))
                property->setAccessMode(VPGEnumClassAttributeAccessMode::ReadWrite);
            else if (vcc::isEqual(attributeToken, attributePrefix + L"NoAccess", true))
                property->setAccessMode(VPGEnumClassAttributeAccessMode::NoAccess);
            else if (vcc::isEqual(attributeToken, attributePrefix + L"Inherit", true))
                property->setIsInherit(true);
            // Property
            else if (vcc::isEqual(attributeToken, attributePrefix + L"Initialize", true)) {
                auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Initialize");
                if (jsonAttributes != nullptr && jsonAttributes->isContainKey(L"Properties")) {
                    for (auto const &element : jsonAttributes->getArray(L"Properties"))
                        property->insertInitializeProperties(element->getArrayElementString());
                }
            }
            // Action
            else if (vcc::isEqual(attributeToken, attributePrefix + L"NoHistory", true))
                property->setIsNoHistory(true);
            else if (vcc::isEqual(attributeToken, attributePrefix + L"ActionResult", true)) {
                auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"ActionResult");
                if (jsonAttributes == nullptr
                    || !((jsonAttributes->isContainKey(L"Redo.Class") && !vcc::isBlank(jsonAttributes->getString(L"Redo.Class")))
                        || (jsonAttributes->isContainKey(L"Undo.Class") && !vcc::isBlank(jsonAttributes->getString(L"Undo.Class")))))
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->getName() + L" has attribute @@ActionResult but missing Attribute \"Redo.Class\" or \"Undo.Class\"");
                if (jsonAttributes->isContainKey(L"Redo.Class") && !vcc::isBlank(jsonAttributes->getString(L"Redo.Class")))
                    property->setActionResultRedoClass(jsonAttributes->getString(L"Redo.Class"));
                if (jsonAttributes->isContainKey(L"Undo.Class") && !vcc::isBlank(jsonAttributes->getString(L"Undo.Class")))
                    property->setActionResultUndoClass(jsonAttributes->getString(L"Undo.Class"));
            }
            // Json
            else if (vcc::isEqual(attributeToken, attributePrefix + L"NoJson", true))
                property->setIsNoJson(true);
            // Command
            else if (vcc::isEqual(attributeToken, attributePrefix + L"Command", true)) {
                std::wstring commandToken = attributePrefix + L"Command";
                commandToken = attribute.substr(commandToken.length());
                vcc::trim(commandToken);
                property->setCommand(commandToken);
            }
        }

        for (auto macro : macroList) {
            auto currentProperty = std::dynamic_pointer_cast<VPGEnumClassAttribute>(property->clone());
            // split macro
            currentProperty->setMacro(macro);

            // virtual
            if (vcc::isStartWith(currentProperty->getMacro(), L"GETCUSTOM("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Getcustom);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SETCUSTOM("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Setcustom);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETCUSTOM_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::GetcustomSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SETCUSTOM_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::SetcustomSptr);

            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETSET("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Getset);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETSET_VALIDATE("))
                currentProperty->setMacroType(VPGEnumClassMacroType::GetsetValidate);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETSET_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::GetsetSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETSET_SPTR_NULL("))
                currentProperty->setMacroType(VPGEnumClassMacroType::GetsetSptrNull);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"GETSET_VALIDATE_SPTR_NULL("))
                currentProperty->setMacroType(VPGEnumClassMacroType::GetsetValidateSptrNull);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"VECTOR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Vector);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"VECTOR_VALIDATE("))
                currentProperty->setMacroType(VPGEnumClassMacroType::VectorValidate);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"VECTOR_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::VectorSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"VECTOR_VALIDATE_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::VectorValidateSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SET("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Set);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SET_VALIDATE("))
                currentProperty->setMacroType(VPGEnumClassMacroType::SetValidate);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SET_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::SetSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"SET_VALIDATE_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::SetValidateSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MAP("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Map);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MAP_VALIDATE("))
                currentProperty->setMacroType(VPGEnumClassMacroType::MapValidate);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MAP_SPTR_R("))
                currentProperty->setMacroType(VPGEnumClassMacroType::MapSptrR);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MAP_VALIDATE_SPTR_R("))
                currentProperty->setMacroType(VPGEnumClassMacroType::MapValidateSptrR);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ORDERED_MAP("))
                currentProperty->setMacroType(VPGEnumClassMacroType::OrderedMap);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ORDERED_MAP_VALIDATE("))
                currentProperty->setMacroType(VPGEnumClassMacroType::OrderedMapValidate);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ORDERED_MAP_SPTR_R("))
                currentProperty->setMacroType(VPGEnumClassMacroType::OrderedMapSptrR);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ORDERED_MAP_VALIDATE_SPTR_R("))
                currentProperty->setMacroType(VPGEnumClassMacroType::OrderedMapValidateSptrR);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MANAGER_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::ManagerSptr);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MANAGER_SPTR_NULL("))
                currentProperty->setMacroType(VPGEnumClassMacroType::ManagerSptrNull);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"MANAGER_SPTR_PARENT("))
                currentProperty->setMacroType(VPGEnumClassMacroType::ManagerSptrParent);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ACTION("))
                currentProperty->setMacroType(VPGEnumClassMacroType::Action);
            else if (vcc::isStartWith(currentProperty->getMacro(), L"ACTION_WITH_ARG_SPTR("))
                currentProperty->setMacroType(VPGEnumClassMacroType::ActionWithArgSptr);

            pos = 0;
            if (currentProperty->getIsManager()) {
                currentProperty->setPropertyType(VPGEnumClassAttributeType::Manager);
    
                currentProperty->setType1(_getType(currentProperty->getMacro(), pos));
    
                if (currentProperty->getMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->setPropertyName(_getPropertyName(currentProperty->getMacro(), pos));
                }
                if (currentProperty->getMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->setDefaultValue(_getDefaultValue(currentProperty->getMacro(), pos));
                }
            } else if (currentProperty->getIsAction()) {
                currentProperty->setPropertyType(VPGEnumClassAttributeType::Action);
    
                currentProperty->setPropertyName(_getType(currentProperty->getMacro(), pos));
                if (currentProperty->getMacro()[pos] == L',')  {
                    pos++;
                    currentProperty->setType1(_getPropertyName(currentProperty->getMacro(), pos));
                }
            } else {
                currentProperty->setPropertyType(VPGEnumClassAttributeType::Property);
    
                if (currentProperty->getIsMap() || currentProperty->getIsOrderedMap()) {
                    currentProperty->setType1(_getType(currentProperty->getMacro(), pos));
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setType2(_getPropertyName(currentProperty->getMacro(), pos));
                    }
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setPropertyName(_getPropertyName(currentProperty->getMacro(), pos));
                    }
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setValidate(_getDefaultValue(currentProperty->getMacro(), pos));
                    }
                } else if (currentProperty->getMacroType() == VPGEnumClassMacroType::Setcustom || currentProperty->getMacroType() == VPGEnumClassMacroType::SetcustomSptr) {
                    currentProperty->setPropertyName(_getType(currentProperty->getMacro(), pos));
    
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setType1(_getPropertyName(currentProperty->getMacro(), pos));
                    }
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setDefaultValue(_getDefaultValue(currentProperty->getMacro(), pos));
                    }
                } else {
                    currentProperty->setType1(_getType(currentProperty->getMacro(), pos));
    
                    if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setPropertyName(_getPropertyName(currentProperty->getMacro(), pos));
                    }
                    if (currentProperty->getMacroType() == VPGEnumClassMacroType::GetsetValidate) {
                        if (currentProperty->getMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->setDefaultValue(_getPropertyName(currentProperty->getMacro(), pos));
                        }
                        if (currentProperty->getMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->setValidate(_getDefaultValue(currentProperty->getMacro(), pos));
                        }
                    } else if (currentProperty->getIsHavingValidate() ) {
                        if (currentProperty->getMacro()[pos] == L',')  {
                            pos++;
                            currentProperty->setValidate(_getDefaultValue(currentProperty->getMacro(), pos));
                        }
                    } else if (currentProperty->getMacro()[pos] == L',')  {
                        pos++;
                        currentProperty->setDefaultValue(_getDefaultValue(currentProperty->getMacro(), pos));
                    }
                }
            }
            properties.push_back(currentProperty);
        }
    CATCH
}

std::wstring VPGEnumClassReader::_getCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const
{
    std::wstring result = L"";
    TRY
        vcc::getNextCharPos(cppCode, pos, true);
        while (vcc::isStartWith(cppCode, L"//", pos) || vcc::isStartWith(cppCode, L"/*", pos)) {
            std::wstring tmpCmd = vcc::getNextQuotedString(cppCode, pos, { L";", L"{", L"\n", L" ", L"/*", L"//" }, { L"/*", L"//"}, {L"*/", L"\n"}, { L"", L"" }, { L"/*", L"//"});
            vcc::trim(tmpCmd);
            if (!result.empty())
                result += L"\r\n";
            if (vcc::isStartWith(tmpCmd, L"//")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 2); // only minus prefix as already vcc::trim
            } else if (vcc::isStartWith(tmpCmd, L"/*")) {
                tmpCmd = tmpCmd.substr(2, tmpCmd.length() - 4);
            }
            vcc::trim(tmpCmd);
            result += tmpCmd;
            size_t currentPos = pos;
            if (isClassCommand) {
                size_t nextNewLinePos = cppCode.find(L"\n", pos + 1);
                vcc::getNextCharPos(cppCode, pos, false);
                if (nextNewLinePos != std::wstring::npos && pos != std::wstring::npos
                    && nextNewLinePos < pos && (vcc::isStartWith(cppCode, L"//", pos) || vcc::isStartWith(cppCode, L"/*", pos))) {
                    result = L"";
                }
                pos = currentPos;
            }
            vcc::getNextCharPos(cppCode, pos, false);
        }
        vcc::trim(result);
        // need to back 1 char as used vcc::getNextCharPos
        pos--;
    CATCH
    return result;
}

void VPGEnumClassReader::_parseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass> enumClass) const
{
    TRY
        _EnumValue = -1;
        while (pos < cppCode.size()) {
            if (vcc::isStartWith(cppCode, L"//", pos) || vcc::isStartWith(cppCode, L"/*", pos)) {
                _getCommand(cppCode, false, pos);
                continue;
            }
            std::wstring enumName = _getEnum(cppCode, pos);
            vcc::trim(enumName);
            if (enumName.empty())
                break;
            
            vcc::getNextCharPos(cppCode, pos, false);
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
                vcc::getNextCharPos(cppCode, pos, false);
            }
            _EnumValue++;

            if (cppCode[pos] == L',')
                vcc::getNextCharPos(cppCode, pos, false);
            
            std::vector<std::shared_ptr<VPGEnumClassAttribute>> properties;
            if (vcc::isStartWith(cppCode, L"//", pos) || vcc::isStartWith(cppCode, L"/*", pos)) {
                _assignEnumClassProperty(enumClass.get(), _getCommand(cppCode, false, pos), properties);
                vcc::getNextCharPos(cppCode, pos, false);
            }
            if (cppCode[pos] == L',')
                vcc::getNextCharPos(cppCode, pos, false);

            if (properties.empty())
                properties.push_back(std::make_shared<VPGEnumClassAttribute>());
            
            for (auto property : properties) {
                property->setEnum(enumName);
                property->setEnumValue(_EnumValue);
            }
            enumClass->getProperties().insert(enumClass->getProperties().end(), properties.begin(), properties.end());
            
            if (cppCode[pos] == L'}') {
                pos--;
                return;
            }
        }
    CATCH
}

bool VPGEnumClassReader::_parseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const
{
    TRY
        if (!vcc::isStartWith(cppCode, L"enum", pos))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _getErrorMessage(cppCode, pos, L"enum missing."));
            
        pos += 4; // length of "enum"
        vcc::getNextCharPos(cppCode, pos, false);
        if (vcc::isStartWith(cppCode, L"class", pos)) {
            pos += 4; // length of "class"
            vcc::getNextCharPos(cppCode, pos, false);
        }
        if (!std::iswalpha(cppCode[pos]))
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _getErrorMessage(cppCode, pos, L"Class Name missing."));

        enumClass->setName(_getEnum(cppCode, pos));
        vcc::getNextCharPos(cppCode, pos, false);

        if (vcc::isStartWith(cppCode, L"//", pos)) {
            enumClass->setCommand(_getCommand(cppCode, false, pos));
            vcc::getNextCharPos(cppCode, pos, false);
        } else if (vcc::isStartWith(cppCode, L"/*", pos)) {
            enumClass->setCommand(_getCommand(cppCode, false, pos));
            vcc::getNextCharPos(cppCode, pos, false);
        }
        if (!enumClass->getCommand().empty()) {
            std::vector<std::wstring> attributes = getAttribute(enumClass->getCommand());
            std::wstring command = L"";
            for (auto const &attribute : attributes) {
                if (vcc::isStartWith(attribute, attributePrefix + L"Form", 0, true)) {
                    enumClass->setType(VPGEnumClassType::Form);
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"ActionArgument", 0, true)) {
                    enumClass->setType(VPGEnumClassType::ActionArgument);
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Result", 0, true)) {
                    enumClass->setType(VPGEnumClassType::Result);
                    command = L"";
                }  else if (vcc::isStartWith(attribute, attributePrefix + L"Inherit", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Inherit");
                    assert(jsonAttributes != nullptr);
                    std::wstring className = jsonAttributes->getString(L"Class");
                    if (vcc::isBlank(className))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Enum Class " + enumClass->getName() + L" has attribute @@Inherit but missing Attribute \"Class\"");
                    enumClass->setInheritClass(className);
                
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Log", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Log");
                    if (jsonAttributes != nullptr)
                        enumClass->setIsLogConfigIndependent(jsonAttributes->getBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Action", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Action");
                    if (jsonAttributes != nullptr)
                        enumClass->setIsActionManagerIndependent(jsonAttributes->getBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Thread", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Thread");
                    if (jsonAttributes != nullptr)
                        enumClass->setIsThreadManagerIndependent(jsonAttributes->getBool(L"IsIndependent"));
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Json", 0, true)) {
                    enumClass->setIsJson(true);
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Json");
                    if (jsonAttributes != nullptr) {
                        for (auto const &key : jsonAttributes->getKeys())
                            enumClass->insertJsonAttributesAtKey(key, jsonAttributes->getString(key));
                    }
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Include", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Include");
                    if (jsonAttributes != nullptr) {
                        if (jsonAttributes->isContainKey(L"SystemFiles"))
                            for (auto const &element : jsonAttributes->getArray(L"SystemFiles"))
                                enumClass->insertIncludeSystemFiles(element->getArrayElementString());
                        if (jsonAttributes->isContainKey(L"CustomFiles"))
                            for (auto const &element : jsonAttributes->getArray(L"CustomFiles"))
                                enumClass->insertIncludeCustomFiles(element->getArrayElementString());
                    }
                    command = L"";
                }  else if (vcc::isStartWith(attribute, attributePrefix + L"Private", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Private");
                    if (jsonAttributes != nullptr && jsonAttributes->isContainKey(L"Properties")) {
                        auto element = jsonAttributes->getObject(L"Properties");
                        if (element != nullptr) {
                            for (auto const &key : element->getKeys())
                                enumClass->insertPrivatePropertiesAtKey(key, element->getString(key));
                        }
                    }
                    command = L"";
                }  else if (vcc::isStartWith(attribute, attributePrefix + L"Protected", 0, true)) {
                    auto jsonAttributes = getJsonAttributes(attribute, attributePrefix + L"Protected");
                    if (jsonAttributes != nullptr && jsonAttributes->isContainKey(L"Properties")) {
                        auto element = jsonAttributes->getObject(L"Properties");
                        if (element != nullptr) {
                            for (auto const &key : element->getKeys())
                                enumClass->insertProtectedPropertiesAtKey(key, element->getString(key));
                        }
                    }
                    command = L"";
                } else if (vcc::isStartWith(attribute, attributePrefix + L"Command", 0, true)) {
                    std::wstring commandToken = attributePrefix + L"Command";
                    commandToken = attribute.substr(commandToken.length());
                    vcc::trim(commandToken);
                    command = commandToken;
                }
                
                enumClass->setCommand(command);
            }
        }
        if (cppCode[pos] == L';') {
            return false;
        } else if (cppCode[pos] != L'{')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _getErrorMessage(cppCode, pos, L"{ missing."));
        vcc::getNextCharPos(cppCode, pos, false);

        _parseProperties(cppCode, pos, enumClass);
        vcc::getNextCharPos(cppCode, pos, false);

        if (cppCode[pos] != L'}')
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, _getErrorMessage(cppCode, pos, L"} missing."));
    CATCH
    return true;
}

std::wstring VPGEnumClassReader::getCppCodeLine(const std::wstring &str, size_t &pos, bool fromCurrentPos) const
{
    if (!fromCurrentPos)
        pos++;
    size_t startPos = pos;
    TRY
        pos = vcc::find(str, L"\n", pos);
        std::wstring tailingSubstr = vcc::getTailingSubstring(str.substr(startPos, pos - startPos + 1), 3);
        if (vcc::isEndWith(tailingSubstr, L"\\\r\n") || vcc::isEndWith(tailingSubstr, L"\\\n")) {
            getCppCodeLine(str, pos, false);
        }
        if (pos == std::wstring::npos)
            pos = str.length();
    CATCH
    return str.substr(startPos, pos - startPos + 1);
}

void VPGEnumClassReader::parseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        size_t pos = 0;
        std::wstring currentCommand = L"";
        bool isNamespaceTriggered = false;
        vcc::getNextCharPos(cppCode, pos, true);
        while (pos < cppCode.size()) {
            size_t commandIndex = isCommand(cppCode, pos);
            if (commandIndex != std::wstring::npos) {
                std::wstring tmpCmd = _getCommand(cppCode, true, pos);
                vcc::trim(tmpCmd);
                if (!currentCommand.empty())
                    currentCommand += L"\r\n";
                currentCommand += tmpCmd;
            } else {
                size_t previousPos = pos;
                std::wstring nextToken = vcc::getNextString(cppCode, pos, _Delimiter, _OpenCommandAndQuotes, _CloseCommandAndQuotes);
                if (!vcc::isBlank(nextToken)) {
                    if (isNamespaceTriggered) {
                        // namespace name
                        vcc::trim(nextToken);
                        pos++;
                        std::wstring quoteStr = vcc::getNextQuotedString(cppCode, pos, { L";" } );
                        vcc::trim(quoteStr);
                        if (!quoteStr.empty()) {
                            std::vector<std::shared_ptr<VPGEnumClass>> tmpClassList;
                            parseCustom(quoteStr.substr(1, quoteStr.length() - 2), nextToken, tmpClassList);
                            if (!currentNamespace.empty()) {
                                for (auto &tmpClass : tmpClassList) {
                                    tmpClass->setName(currentNamespace + L"::" + tmpClass->getName());
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
                            getCppCodeLine(cppCode, pos, true);
                        } else if (nextToken == L"enum") {
                            pos = previousPos;
                            auto enumClass = std::make_shared<VPGEnumClass>();
                            enumClass->setCommand(currentCommand);
                            bool isFullEnumClass = _parseClass(cppCode, pos, enumClass);
                            if (!currentNamespace.empty())
                                enumClass->setName(currentNamespace + L"::" + enumClass->getName());
                            if (isFullEnumClass)
                                results.push_back(enumClass);
                        }
                    }
                    currentCommand = L"";
                }
            }
            vcc::getNextCharPos(cppCode, pos, false);
        }
    CATCH
}

void VPGEnumClassReader::parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const
{
    TRY
        std::unique_lock lock(_mutex);
        parseCustom(cppCode, L"", results);
    CATCH
}