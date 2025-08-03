#include "vpg_object_file_generation_service.hpp"

#include <map>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"
#include "string_helper.hpp"

#include "vpg_action_file_generation_service.hpp"
#include "vpg_class_helper.hpp"
#include "vpg_cpp_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_config.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_tag_helper.hpp"

#define LOG_ID L"Object File Generation"

std::wstring VPGObjectFileGenerationService::getCloneFunction(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping, const bool &isCpp)
{
    std::wstring result = L"";
    TRY
        bool isContentNeeded = false;
        result += L"\r\n";
        std::wstring indent = !isCpp ? (INDENT + INDENT) : L"";

        if (enumClass->getType() == VPGEnumClassType::Form) {
            if (isCpp) {
                result += indent + L"std::shared_ptr<vcc::IObject> " + className + L"::clone() const\r\n";
                isContentNeeded = true;
            } else {
                result += indent + L"virtual std::shared_ptr<vcc::IObject> clone() const override;\r\n";
                isContentNeeded = false;
            }
        } else {
            result += indent + L"virtual std::shared_ptr<vcc::IObject> clone() const override\r\n";
            isContentNeeded = true;
        }

        if (isContentNeeded) {
            result += indent + L"{\r\n";
            std::map<std::wstring, std::wstring> cloneObjs;
            std::wstring cloneContent = L"";
            auto propertyList = enumClass->getPrivateProperties();
            propertyList.insert(enumClass->getProtectedProperties().begin(), enumClass->getProtectedProperties().end());
            for (auto const &property : propertyList) {
                std::wstring type = vcc::isContain(property.second, L"=") ? vcc::splitString(property.second, { L"=" })[0] : property.second;
                vcc::trim(type);
                if (!vcc::isCapital(type))
                    continue;
                if (enumClassMapping.find(type) == enumClassMapping.end())
                    cloneObjs.insert({ property.first, L"obj->" + property.first + L" = std::dynamic_pointer_cast<" + type + L">(this->" + property.first + L"->clone())" });
            }
            for (auto const &property : enumClass->getProperties()) {
                // handle enum without macro case
                if (property->getPropertyType() != VPGEnumClassAttributeType::Property || property->getIsCustom())
                    continue;
                if ((!property->getType1().empty() && vcc::isCapital(getTypeOrClassWithoutNamespace(property->getType1()))) 
                    || (!property->getType2().empty() && vcc::isCapital(getTypeOrClassWithoutNamespace(property->getType2())))) {
                    if (vcc::find(property->getMacro(), L"SPTR") != std::wstring::npos)
                        cloneObjs.insert({ property->getPropertyName(), L"obj->clone" + property->getPropertyName() + L"(this->_" + property->getPropertyName() + (vcc::isStartWith(property->getMacro(), L"GETSET") ? L".get()" : L"" ) + L")" });
                }
            }
            if (!cloneObjs.empty()) {
                result += indent + INDENT + L"auto obj = std::make_shared<" + className + L">(*this);\r\n";
                for (auto const &cloneObj : cloneObjs)
                    result += indent + INDENT + cloneObj.second + L";\r\n";
                result += indent + INDENT + L"return obj;\r\n";
            } else
                result += indent + INDENT + L"return std::make_shared<" + className + L">(*this);\r\n";
            result += indent + L"}\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getConstructorContent(const VPGEnumClass *enumClass, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping, const bool &isHeader)
{
    std::wstring result = L"";
    TRY
        std::vector<std::wstring> initializeList;
        auto propertyList = enumClass->getPrivateProperties();
        propertyList.insert(enumClass->getProtectedProperties().begin(), enumClass->getProtectedProperties().end());
        // Property initialization First, then override by Macro initialization
        for (auto property : propertyList) {
            std::wstring type = vcc::isContain(property.second, L"=") ? vcc::splitString(property.second, { L"=" })[0] : property.second;
            std::wstring defaultValue = vcc::isContain(property.second, L"=") ? vcc::splitString(property.second, { L"=" })[1] : L"";
            vcc::trim(type);
            vcc::trim(defaultValue);
            if (vcc::isCapital(type) && enumClassMapping.find(type) == enumClassMapping.end() && defaultValue != L"nullptr") 
                initializeList.push_back(property.first + L" = " + defaultValue + (!vcc::isEndWith(defaultValue, L";") ? L";" : L""));
        }
        for (auto property : enumClass->getProperties()) {
            if (property->getIsInitializeInClassConstructorNeeded()) {
                if (property->getIsCustom()) {
                    if (property->getInitializeProperties().size() > 0)
                        initializeList.push_back(L"set" + property->getPropertyName() + L"(" + property->getInitializePropertiesAtIndex(0) + L");");
                } else {
                    if (property->getIsCollection()) {
                        for (auto const &element : property->getInitializeProperties())
                            initializeList.push_back(L"insert" + property->getPropertyName() + L"(" + element + L");");
                    } else if (property->getIsObject()){
                        if (property->getInitializeProperties().size() > 0)
                            initializeList.push_back(L"_" + property->getPropertyName() + L" = std::make_shared<" + property->getType1() + L">(" + vcc::concat(property->getInitializeProperties(), L", ") + L");");
                        else
                            initializeList.push_back(L"_" + property->getPropertyName() + L" = std::make_shared<" + property->getType1() + L">(" + property->getDefaultValue() + L");");
                    } else {
                        if (property->getInitializeProperties().size() > 0)
                            initializeList.push_back(L"_" + property->getPropertyName() + L" = " + property->getInitializePropertiesAtIndex(0) + L";");
                    }
                }
            }
        }
        for (auto const &str : initializeList)
            result += INDENT + INDENT + (isHeader ? INDENT : L"") + str + L"\r\n";
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::getObjectToJsonEnumSwitch(const std::wstring &switchVariable, const std::wstring &returnVariable,
    const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::vector<std::wstring> result;
    TRY
        result.push_back(L"std::wstring " + returnVariable + L" = L\"\";");
        result.push_back(L"switch (" + switchVariable + L")");
        result.push_back(L"{");

        std::shared_ptr<VPGEnumClass> enumEnumClass = nullptr;
        if (enumClassMapping.find(type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(type);
        } else if (enumClassMapping.find(L"vcc::" + type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(L"vcc::" + type);
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"VPGObjectFileGenerationService::GenerateCpp Enum Class " + type + L" cannot found");
        for (auto const &enumEnumClassProperty : enumEnumClass->getProperties()) {
            result.push_back(L"case " + enumEnumClass->getName() + L"::" + enumEnumClassProperty->getEnum() + L":");
            result.push_back(INDENT + returnVariable + L" = L\"" + enumEnumClassProperty->getEnum() + L"\";");
            result.push_back(INDENT + L"break;");
        }
        result.push_back(L"default:");
        result.push_back(INDENT + L"assert(false);");
        result.push_back(INDENT + L"break;");
        result.push_back(L"}");
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::getJsonToObjectEnumSwitch(const std::wstring &propertyName, const bool isKey,
    const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::vector<std::wstring> result;
    TRY
        std::wstring jsonEnumValue = isKey ? L"keyEnum" : L"valueEnum";
        std::wstring jsonStr = jsonEnumValue + L"Str";
        std::wstring jsonStrUpper = jsonStr + L"Upper";

        result.push_back(INDENT + L"std::wstring " + jsonStr + L" = " + (isKey ? L"key;" : (L"json->getString(vcc::convertNamingStyle(L" + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, propertyName) + L", namestyle, vcc::NamingStyle::PascalCase));")));
        result.push_back(INDENT + L"std::wstring " + jsonStrUpper + L" = " + jsonStr + L";");
        result.push_back(INDENT + L"vcc::toUpper(" + jsonStrUpper + L");");
        result.push_back(INDENT + L"int64_t " + jsonEnumValue + L" = -1;");

        std::shared_ptr<VPGEnumClass> enumEnumClass = nullptr;
        if (enumClassMapping.find(type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(type);
        } else if (enumClassMapping.find(L"vcc::" + type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(L"vcc::" + type);
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"VPGObjectFileGenerationService::GenerateCpp Enum Class " + type + L" cannot found");
        
        bool isStart = true;
        for (auto const &enumEnumClassProperty : enumEnumClass->getProperties()) {
            std::wstring ifPrefix = isStart ? L"" : L"else ";
            std::wstring enumNameUpper = enumEnumClassProperty->getEnum();
            vcc::toUpper(enumNameUpper);
            result.push_back(INDENT + ifPrefix + L"if (" + jsonStrUpper + L" == L\"" + enumNameUpper + L"\")");
            result.push_back(INDENT + INDENT + jsonEnumValue + L" = static_cast<int64_t>(" + enumEnumClass->getName() + L"::" + enumEnumClassProperty->getEnum() + L");");
            isStart = false;
        }
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::getObjectToJson(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &propertyName, const bool &isArray, const bool &isMap)
{
    std::vector<std::wstring> result;
    TRY
        std::wstring arrayStr = isArray ? L"Array" : L"";
        std::wstring convertedPropertyNameNoComma = isMap ? propertyName : (!isArray ? (L"vcc::convertNamingStyle(L" + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, propertyName) + L", vcc::NamingStyle::PascalCase, namestyle)") : L"");
        std::wstring convertedPropertyName = !convertedPropertyNameNoComma.empty() ? (convertedPropertyNameNoComma + L", ") : L"";
        std::wstring currentPropertyName = L"";
        if (isMap)
            currentPropertyName = L"element.second";
        else if (isArray)
            currentPropertyName = propertyName;
        else
            currentPropertyName = L"get" + propertyName + L"()";
            
        if (vcc::isContain(macro, L"SPTR")) {
            // Object
            result.push_back(L"if (" +currentPropertyName + L" != nullptr)");
            result.push_back(INDENT + parentName + L"->add" + arrayStr + L"Object(" + convertedPropertyName + currentPropertyName + L"->ToJson());");
            result.push_back(L"else");
            result.push_back(INDENT + parentName + L"->add" + arrayStr + L"Null(" + convertedPropertyNameNoComma + L");");
        } else if (vcc::isCapital(getTypeOrClassWithoutNamespace(type))) {
            // Enum
            std::wstring tmpPropertyName = L"";
            if (isMap) {
                tmpPropertyName = L"valueStr";
            } else {
                tmpPropertyName = propertyName.substr(0, 1);
                vcc::toLower(tmpPropertyName);
                if (propertyName.size() > 1)
                    tmpPropertyName += propertyName.substr(1);
                tmpPropertyName += L"ValueStr";
            }
            std::vector<std::wstring> enumSwitchStr = getObjectToJsonEnumSwitch(currentPropertyName, tmpPropertyName, type, enumClassMapping);
            result.insert(result.end(), enumSwitchStr.begin(), enumSwitchStr.end());
            result.push_back(parentName + L"->add" + arrayStr + L"String(" + convertedPropertyName  + tmpPropertyName +L");");
        } else if (type == L"bool")
            result.push_back(parentName + L"->add" + arrayStr + L"Bool(" + convertedPropertyName + currentPropertyName + L");");
        else if (vcc::isContain(type, L"int")
            || vcc::isContain(type, L"short")
            || vcc::isContain(type, L"long")
            || type == L"size_t"
            || type == L"time_t")
            result.push_back(parentName + L"->add" + arrayStr + L"Int(" + convertedPropertyName  + currentPropertyName + L");");
        else if (type == L"double" || type == L"float")
            result.push_back(parentName + L"->add" + arrayStr + L"Double(" + convertedPropertyName + currentPropertyName + L", decimalPlaces >= 0 ? decimalPlaces : getDecimalPlaces(" + currentPropertyName + L"));");
        else if (type == L"string" || type == L"std::string")
            result.push_back(parentName + L"->add" + arrayStr + L"String(" + convertedPropertyName + L"str2wstr(" + currentPropertyName + L"));");
        else if (type == L"wstring" || type == L"std::wstring")
            result.push_back(parentName + L"->add" + arrayStr + L"String(" + convertedPropertyName  + currentPropertyName + L");");
        else
            result.push_back(parentName + L"->add" + arrayStr + L"String(" + convertedPropertyName + L"std::to_wstring(" + currentPropertyName + L"));");
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getJsonToObjectMapKey(const std::wstring &type)
{
    std::wstring result = L"";
    TRY
        if (type == L"bool") {
            result = L"key == \"true\" ? true : false";
        } else if (type == L"char"
            || type == L"byte") {
            result = L"wstr2str(key)[0]";
        } else if (type == L"wchar_t") {
            result = L"key[0]";
        } else if (vcc::isContain(type, L"int")
            || vcc::isContain(type, L"short")
            || vcc::isContain(type, L"long")
            || type == L"size_t"
            || type == L"time_t")
            result = L"std::stoi(key)";
        else if (type == L"float" || type == L"double") {
            result = L"std::stod(key)";
        } else if (type == L"string" || type == L"std::string") {
            result = L"wstr2str(key)";
        } else if (type == L"wstring" || type == L"std::wstring") {
            result = L"key";
        } else if (vcc::isCapital(getTypeOrClassWithoutNamespace(type))) {
            result = L"static_cast<" + type + L">(keyEnum)";
        }
        else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + type);
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::getJsonToObject(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &mapKeyType, const std::wstring &propertyName, const bool &isArray, const bool &isMap)
{
    std::vector<std::wstring> result;
    TRY
        bool ifCaseNeedKey = vcc::isCapital(getTypeOrClassWithoutNamespace(mapKeyType));
        bool ifCaseNeedValue = !vcc::isContain(macro, L"SPTR") && vcc::isCapital(getTypeOrClassWithoutNamespace(type)); // for enum only
        std::wstring arrayStr = isArray ? L"Array" : L"";
        std::wstring convertedPropertyNameForGeneral = isMap ? (ifCaseNeedKey ? L"keyEnumStr" : L"key") : (isArray ? L"" : (L"vcc::convertNamingStyle(L" + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, propertyName) + L", namestyle, vcc::NamingStyle::PascalCase)"));
        std::wstring arrayElementStr = convertedPropertyNameForGeneral.empty() ? L"ArrayElement" : L"";
        std::wstring currentParentName = isMap ? L"tmpObject" : parentName;
        std::wstring insertPrefix = (isMap || isArray) ? (L"insert" + propertyName + (isMap ? L"AtKey" : L"") + L"(" + (isMap ? (getJsonToObjectMapKey(mapKeyType) + L", ") : L"")) : (L"set" + propertyName + L"(");
        std::wstring insertSuffix = L");";
        std::wstring indentPrefix = INDENT;
        if (ifCaseNeedKey || ifCaseNeedValue) {
            if (ifCaseNeedValue) {
                std::vector<std::wstring> enumSwitchStr = getJsonToObjectEnumSwitch(propertyName, false, type, enumClassMapping);
                result.insert(result.end(), enumSwitchStr.begin(), enumSwitchStr.end());
            }
            std::wstring ifContent = ifCaseNeedValue ? L" && " : L"";
            ifContent = ifCaseNeedKey ? (L"keyEnum > -1" + ifContent) : L"";
            ifContent += ifCaseNeedValue ? L"valueEnum > -1" : L"";
            result.push_back(INDENT + L"if (" + ifContent + L")");
            indentPrefix += INDENT;
        }
        if (vcc::isContain(macro, L"SPTR")) {
            // Object
            result.push_back(indentPrefix + (isMap || isArray ? (L"get" + propertyName + L"()") : L"tmpObject") + L"->deserializeJson(" + currentParentName + L"->getObject(vcc::convertNamingStyle(L" + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, propertyName) + L", namestyle, vcc::NamingStyle::PascalCase)));");
        } else if (vcc::isCapital(getTypeOrClassWithoutNamespace(type))) {
            // Enum
            result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(valueEnum)" + insertSuffix);
        } else {
            if (type == L"bool")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->get" + arrayElementStr + L"Bool(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (type == L"char")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->get" + arrayElementStr + L"Char(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (type == L"wchar_t")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->get" + arrayElementStr + L"Wchar(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (vcc::isContain(type, L"int")
                || vcc::isContain(type, L"short")
                || vcc::isContain(type, L"long")
                || type == L"size_t"
                || type == L"time_t")
                result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(" + currentParentName + L"->get" + arrayElementStr + L"Int64(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"double" || type == L"float")
                result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(" + currentParentName + L"->get" + arrayElementStr + L"Double(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"string" || type == L"std::string")
                result.push_back(indentPrefix + insertPrefix + L"wstr2str(" + currentParentName + L"->get" + arrayElementStr + L"String(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"wstring" || type == L"std::wstring")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->get" + arrayElementStr + L"String(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"VPGObjectFileGenerationService::GenerateCpp Unknown type: " + type);
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className)
{
    TRY
        if (vcc::isContain(projectClassIncludeFiles, className))
            return projectClassIncludeFiles.at(className);
        else
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Include file of Class " + className + L" NOT FOUND");
    CATCH
    return L"";
}

void VPGObjectFileGenerationService::getHppIncludeFiles(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
    bool &isContainForm,
    std::set<std::wstring> &systemFileList,
    std::set<std::wstring> &projectFileList,
    std::set<std::wstring> &abstractClassList,
    std::set<std::wstring> &abstractEnumClassList,
    std::set<std::wstring> &classInCurrentFileList)
{ 
    TRY
        // TODO: need to enable to check all systemn function
        // need to skip all code in command
        // Mac: all next tokens after class _LIBCPP_TEMPLATE_VIS under path /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1
        // Window: C:\msys64\mingw64\include\c++\12.2.0

        projectFileList.insert(L"class_macro.hpp");
        projectFileList.insert(L"object_type.hpp");
        
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L""))
                continue;

            // force insert include file list
            for (auto const &fileName : enumClass->getIncludeSystemFiles())
                systemFileList.insert(fileName);
            for (auto const &fileName : enumClass->getIncludeCustomFiles())
                projectFileList.insert(fileName);

            std::wstring className = getClassNameFromPropertyClassName(enumClass->getName());
            classInCurrentFileList.insert(className);
            // ------------------------------------------------------------------------------------------ //
            //                               Class Level                                                  //
            // ------------------------------------------------------------------------------------------ //
            switch (enumClass->getType())
            {
            case VPGEnumClassType::ActionArgument:
                projectFileList.insert(L"base_action_argument.hpp");
                break;
            case VPGEnumClassType::Form:
                systemFileList.insert(L"memory");

                projectFileList.insert(L"base_form.hpp");
                projectFileList.insert(L"i_result.hpp");
                isContainForm = true;
                break;
            case VPGEnumClassType::Object:
                projectFileList.insert(L"base_object.hpp");
                break;
            case VPGEnumClassType::Result:
                systemFileList.insert(L"string");
                systemFileList.insert(L"memory");

                projectFileList.insert(L"base_result.hpp");
                projectFileList.insert(L"exception_type.hpp");
                break;
            default:
                break;
            }

            // Json
            if (enumClass->getIsJson()) {
                projectFileList.insert(L"base_json_object.hpp");
                projectFileList.insert(L"json.hpp");
                projectFileList.insert(L"i_document.hpp");
            }
            if (!vcc::isBlank(enumClass->getInheritClass())) {
                std::wstring inheritClass = enumClass->getInheritClass();
                // remove template
                size_t pos = vcc::find(inheritClass, L"<");
                if (pos != std::wstring::npos) {
                    inheritClass = inheritClass.substr(0, pos);
                }
                projectFileList.insert(getProjectClassIncludeFile(projectClassIncludeFiles, inheritClass));
            }
            
            // ------------------------------------------------------------------------------------------ //
            //                               Class extra Property                                         //
            // ------------------------------------------------------------------------------------------ //
            std::set<std::wstring> extraPropertyList;
            for (auto const &property : enumClass->getPrivateProperties()) {
                auto type = vcc::splitString(property.second, { L"=" }).front();
                vcc::trim(type);
                extraPropertyList.insert(type);
            }
            for (auto const &property : enumClass->getProtectedProperties()) {
                auto type = vcc::splitString(property.second, { L"=" }).front();
                vcc::trim(type);
                extraPropertyList.insert(type);
            }
            for (auto const &extraProperty : extraPropertyList) {
                if (vcc::isCapital(getTypeOrClassWithoutNamespace(extraProperty))) {
                    std::wstring includeFile = VPGObjectFileGenerationService::getProjectClassIncludeFile(projectClassIncludeFiles, extraProperty);
                    if (!includeFile.empty()) {
                        projectFileList.insert(includeFile);
                        // if not enum class, then need to add memory for shared_ptr
                        if (enumClassMapping.find(extraProperty) == enumClassMapping.end())
                            systemFileList.insert(L"memory");
                    }
                    else
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Class " + extraProperty + L" NOT FOUND");
                } else {
                    // TODO: need to enable to check all systemn function
                    // system type
                    if (vcc::countSubstring(extraProperty, L"string") > 0)
                        systemFileList.insert(L"string");
                }
            }

            // ------------------------------------------------------------------------------------------ //
            //                               Property Level                                               //
            // ------------------------------------------------------------------------------------------ //
            for (auto const &property : enumClass->getProperties()) {
                // handle enum without macro case
                if (property->getPropertyType() == VPGEnumClassAttributeType::NA || property->getPropertyType() == VPGEnumClassAttributeType::Action || property->getIsInherit())
                    continue;

                if (property->getIsObject())
                    systemFileList.insert(L"memory");

                for (auto i = 0; i < 2; i++) {
                    std::wstring type = i != 0 ? property->getType2() : property->getType1();
                    if (vcc::isBlank(type))
                        continue;

                    if (i != 0 ? property->getIsType2Custom() : property->getIsType1Custom()) {
                        if (property->getIsObject()) {
                            std::wstring includeFile = VPGObjectFileGenerationService::getProjectClassIncludeFile(projectClassIncludeFiles, type);
                            if (!includeFile.empty())
                                projectFileList.insert(includeFile);
                            else
                                abstractClassList.insert(type);
                        } else {
                            std::wstring includeFile = VPGObjectFileGenerationService::getProjectClassIncludeFile(projectClassIncludeFiles, type);
                            if (!includeFile.empty())
                                projectFileList.insert(includeFile);
                            else
                                abstractEnumClassList.insert(type);
                        }
                    } else {
                        // TODO: need to enable to check all systemn function
                        // system type
                        if (vcc::countSubstring(type, L"string") > 0)
                            systemFileList.insert(L"string");
                    }
        
                }
                
                if (property->getPropertyType() == VPGEnumClassAttributeType::Manager && vcc::isStartWith(property->getMacro(), L"MANAGER_SPTR_PARENT")) {
                    std::wstring type = property->getDefaultValue();
                    if (!type.empty() && vcc::isCapital(type)) {
                        std::wstring includeFile = VPGObjectFileGenerationService::getProjectClassIncludeFile(projectClassIncludeFiles, type);
                        if (!includeFile.empty())
                            projectFileList.insert(includeFile);
                        else
                            abstractClassList.insert(type);
                    }
                }
            }
        }
    CATCH
}

std::wstring VPGObjectFileGenerationService::getHppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::wstring result = L"";
    TRY
        // Constructor
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += INDENT + INDENT + className + L"();\r\n";
        } else {
            std::wstring initializeStr = getConstructorContent(enumClass, enumClassMapping, true);
            if (enumClass->getType() == VPGEnumClassType::Result) {
                result += INDENT + INDENT + className + L"() : " + className + L"(ExceptionType::NoError, L\"\") {}\r\n"
                    + INDENT + INDENT + className + L"(const ExceptionType &exceptionType, const std::wstring &errorMessage) : BaseResult(ObjectType::" +  className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L", exceptionType, errorMessage)";
                if (!initializeStr.empty()) {
                    result += L"\r\n"
                        + INDENT + INDENT + L"{\r\n"
                        + initializeStr
                        + INDENT + INDENT + L"}\r\n";
                } else
                    result += L" {}\r\n";
            } else {
                if (!vcc::isBlank(enumClass->getInheritClass()))
                    result += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"()\r\n"
                        + INDENT + INDENT + L"{\r\n"
                        + INDENT + INDENT + INDENT + L"_ObjectType = ObjectType::" +  className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L";\r\n"
                        + initializeStr
                        + INDENT + INDENT + L"}\r\n";
                else {
                    result += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"(ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L")";
                    if (!initializeStr.empty())
                        result += L"\r\n"
                            + INDENT + INDENT + L"{\r\n"
                            + initializeStr
                            + INDENT + INDENT;
                    else
                        result += L" {";
                    result += L"}\r\n";
                    if (!initializeStr.empty())
                        result += L"\r\n";
                }
            }
        }
        // Destructor
        result += INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppProperties(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::wstring result = L"";
    TRY
        // extra properties - maybe used in custom function, need to be generated first
        if (!enumClass->getPrivateProperties().empty())
            result += INDENT + L"private:\r\n";
        for (auto const &property : enumClass->getPrivateProperties()) {
            std::wstring type = L"";
            std::wstring defaultValue = L"";
            if (vcc::isContain(property.second, L"=")) {
                type = vcc::splitString(property.second, { L"=" })[0];
                defaultValue = vcc::splitString(property.second, { L"=" })[1];
            } else
                type = property.second;
            vcc::trim(type);
            vcc::trim(defaultValue);
            if (vcc::isCapital(type) && enumClassMapping.find(type) == enumClassMapping.end()) {
                type = L"std::shared_ptr<" + type + L">";
                defaultValue = L"nullptr";
            }
            result += INDENT + INDENT + L"mutable " + type + L" " + property.first + (!defaultValue.empty() ? (L" = " + defaultValue) : L"") + (!vcc::isEndWith(defaultValue, L";") ? L";" : L"") + L"\r\n";
        }

        if (!result.empty())
            result += L"\r\n";
        if (!enumClass->getProtectedProperties().empty())
            result += INDENT + L"protected:\r\n";
        for (auto const &property : enumClass->getProtectedProperties()) {
            std::wstring type = L"";
            std::wstring defaultValue = L"";
            if (vcc::isContain(property.second, L"=")) {
                type = vcc::splitString(property.second, { L"=" })[0];
                defaultValue = vcc::splitString(property.second, { L"=" })[1];
            } else
                type = property.second;
            vcc::trim(type);
            vcc::trim(defaultValue);
            if (vcc::isCapital(type) && enumClassMapping.find(type) == enumClassMapping.end()) {
                type = L"std::shared_ptr<" + type + L">";
                defaultValue = L"nullptr";
            }
            result += INDENT + INDENT + L"mutable " + type + L" " + property.first + (!defaultValue.empty() ? (L" = " + defaultValue) : L"") + (!vcc::isEndWith(defaultValue, L";") ? L";" : L"") + L"\r\n";
        }

        if (!result.empty())
            result += L"\r\n";
        // generate properties
        for (auto const &property : enumClass->getProperties()) {
            // handle enum without macro case
            // Not generate inherited properties
            if (property->getMacro().empty() || property->getIsInherit())
                continue;
            result += INDENT + property->getMacro() + L"\r\n";
        }

        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + getVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + getVccTagTailerCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppPrivateFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + L"private:\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassPrivateFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassPrivateFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppProtectedFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + L"protected:\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppPublicCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    TRY
        return getCloneFunction(enumClass, className, enumClassMapping, false);
    CATCH
    return L"";
}

std::wstring VPGObjectFileGenerationService::getHppPublicJsonFunctions(const VPGEnumClass *enumClass)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getIsJson())
            result += L"\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<vcc::Json> ToJson() const override;\r\n"
                + INDENT + INDENT + L"virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppPublicFunctions(const VPGEnumClass *enumClass, const VPGConfig *option)
{
    std::wstring result = L"";
    TRY
        switch (enumClass->getType())
        {
        case VPGEnumClassType::Form:
            result += L"\r\n"
                + INDENT + INDENT + L"virtual void initializeComponents() override;\r\n"
                "\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<vcc::IResult> doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> argument) override;\r\n";
            break;
        case VPGEnumClassType::Result:
            if (option->getBehavior() != nullptr && option->getBehavior()->getIsActionResultThrowException())
                result += L"\r\n"
                    + INDENT + INDENT + L"virtual bool isThrowException() const override\r\n"
                    + INDENT + INDENT + L"{\r\n"
                    + INDENT + INDENT + INDENT + L"return true;\r\n"
                    + INDENT + INDENT + L"}\r\n";
            break;
        default:
            break;
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getHppPublicCustomFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassPublicFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassPublicFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;    
}

std::wstring VPGObjectFileGenerationService::GenerateHppClass(const VPGEnumClass* enumClass, const VPGConfig *option, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::wstring result = L"";
    TRY
        std::wstring className = getTypeOrClassWithoutNamespace(getClassNameFromPropertyClassName(enumClass->getName()));
                
        std::wstring inheritClass = L"";
        // Json
        if (enumClass->getIsJson())
            inheritClass += L", public vcc::BaseJsonObject";

        std::wstring baseClassName = L"";
        switch (enumClass->getType())
        {
        case VPGEnumClassType::ActionArgument:
            baseClassName = L"vcc::BaseActionArgument";
            break;
        case VPGEnumClassType::Form:
            baseClassName = L"vcc::BaseForm";
            break;
        case VPGEnumClassType::Result:
            baseClassName = L"vcc::BaseResult";
            break;
        default:
            baseClassName = L"vcc::BaseObject";
            break;
        }
        if (!vcc::isBlank(enumClass->getInheritClass()))
            baseClassName = enumClass->getInheritClass();
        std::wstring baseClassNameWithoutQuote = baseClassName;
        if (vcc::isContain(baseClassNameWithoutQuote, L"<"))
            baseClassNameWithoutQuote = baseClassNameWithoutQuote.substr(0, vcc::find(baseClassNameWithoutQuote, L"<"));

        result += L"\r\n"
            "class " + className + L" : public " + baseClassName + inheritClass + L"\r\n"
            "{\r\n"
            + getHppProperties(enumClass, className, enumClassMapping)
            + getHppPrivateFunctions(enumClass, className)
            + getHppProtectedFunctions(enumClass, className)
            + L"\r\n"
            + INDENT + L"public:\r\n"
            + getHppConstructor(enumClass, option->getProjectPrefix(), className, baseClassNameWithoutQuote, enumClassMapping)
            + getHppPublicCloneFunctions(enumClass, className, enumClassMapping)
            + getHppPublicJsonFunctions(enumClass)
            + getHppPublicFunctions(enumClass, option)
            + getHppPublicCustomFunctions(enumClass, className)
            + L"};\r\n";
    CATCH
    return result;
}

void VPGObjectFileGenerationService::GenerateHpp(const vcc::LogConfig *logConfig,
    const VPGConfig *option,
    const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &objectFilePathHpp,
    const std::wstring &formFilePathHpp,
    const std::wstring &actionFolderPathHpp,
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        if (objectFilePathHpp.empty() && formFilePathHpp.empty())
            return;
        std::set<std::wstring> systemFileList;
        std::set<std::wstring> projectFileList;
        std::set<std::wstring> abstractClassList;
        std::set<std::wstring> abstractEnumClassList;
        std::set<std::wstring> classInCurrentFileList;
        bool isContainForm = false;
        getHppIncludeFiles(projectClassIncludeFiles,
            enumClassMapping,
            enumClassList,
            isContainForm,
            systemFileList,
            projectFileList,
            abstractClassList,
            abstractEnumClassList,
            classInCurrentFileList);
        
        std::wstring classPrefix = option->getProjectPrefix();
        std::wstring filePathHpp = isContainForm && !formFilePathHpp.empty() ? formFilePathHpp : objectFilePathHpp;
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object class file: " + filePathHpp);

        // ------------------------------------------------------------------------------------------ //
        //                               Action  Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::map<std::wstring, std::wstring> actionNameAndActionClassList;
        for (auto const &enumClass : enumClassList)
            VPGActionFileGenerationService::GenerateHpp(logConfig, projectClassIncludeFiles, enumClass.get(), classPrefix, actionFolderPathHpp, actionNameAndActionClassList, systemFileList, projectFileList);        

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Script                                              //
        // ------------------------------------------------------------------------------------------ //
        std::wstring systemFileListStr = L"";
        std::wstring projectFileListStr = L"";
        for (auto const &str : systemFileList)
            systemFileListStr += L"#include <" + str + L">\r\n";

        std::wstring fileHpp = vcc::getFileName(filePathHpp);
        for (auto const &str : projectFileList) {
            if (str == fileHpp)
                continue;
            projectFileListStr +=  L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";
        }
        std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#pragma once\r\n";
        content += !systemFileListStr.empty() ? (L"\r\n" + systemFileListStr) : L"";
        content += !projectFileListStr.empty() ? (L"\r\n" + projectFileListStr) : L"";
        
        if (isContainForm) {
            content += L"\r\n"
                + getVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                + getVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";
        }

        std::wstring classList = L"";
        // for those class that cannot be found in file list
        for (auto const &str : abstractClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            classList += L"class " + str + L";\r\n";
        }
        for (auto const &str : abstractEnumClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            classList +=  L"enum class " + str + L";\r\n";
        }
        if (!classList.empty())
            content += L"\r\n"
                + classList;

        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        // 1. Generate action argument class
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L"") || enumClass->getType() != VPGEnumClassType::ActionArgument)
                continue;
            std::wstring currentNamespace = getNamespaceFromClassName(enumClass->getName());
            if (!vcc::isContain(namespaceClassMapping, currentNamespace))
                namespaceClassMapping.insert({currentNamespace, {}});
            namespaceClassMapping[currentNamespace].push_back(GenerateHppClass(enumClass.get(), option, enumClassMapping));
        }

        // 2. Generate Action
        for (auto const &pair : actionNameAndActionClassList) {
            std::wstring currentNamespace = getNamespaceFromClassName(pair.first);
            if (!vcc::isContain(namespaceClassMapping, currentNamespace))
                namespaceClassMapping.insert({currentNamespace, {}});
            namespaceClassMapping[currentNamespace].push_back(pair.second);
        }
        
        // 3. Generate class
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L"") || enumClass->getType() == VPGEnumClassType::ActionArgument)
                continue;
            
            std::wstring currentNamespace = getNamespaceFromClassName(enumClass->getName());
            if (!vcc::isContain(namespaceClassMapping, currentNamespace))
                namespaceClassMapping.insert({currentNamespace, {}});
            namespaceClassMapping[currentNamespace].push_back(GenerateHppClass(enumClass.get(), option, enumClassMapping));
        }

        // 4. Generate code in namespace
        content += generateCodeWithNamespace(namespaceClassMapping);
        
        // ------------------------------------------------------------------------------------------ //
        //                               Handle VCC Tag                                               //
        // ------------------------------------------------------------------------------------------ //
        if (vcc::isFilePresent(filePathHpp))
            content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathHpp), VPGFileContentSyncMode::Full, L"//");
        
        vcc::lTrim(content);
        vcc::writeFile(filePathHpp, content, true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object class file completed.");
    CATCH
}

void VPGObjectFileGenerationService::getCppIncludeFiles(
    const std::map<std::wstring, std::wstring> &classPathMapping, 
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
    const bool &isContainForm,
    const bool &isContainJson,
    std::set<std::wstring> &systemIncludeFiles,
    std::set<std::wstring> &customIncludeFiles)
{
    if (isContainForm) {
        systemIncludeFiles.insert(L"assert.h");
        systemIncludeFiles.insert(L"memory");
        
        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_result.hpp");

        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L""))
                continue;

            // Property Enum Class
            customIncludeFiles.insert(getProjectClassIncludeFile(classPathMapping, enumClass->getName()));

            // inherit Form
            if (!vcc::isBlank(enumClass->getInheritClass())) {
                std::wstring inheritClass = enumClass->getInheritClass();
                // remove template
                size_t pos = vcc::find(inheritClass, L"<");
                if (pos != std::wstring::npos) {
                    inheritClass = inheritClass.substr(0, pos);
                }
                customIncludeFiles.insert(getProjectClassIncludeFile(classPathMapping, inheritClass));
            } else {
                customIncludeFiles.insert(L"base_form.hpp");
            }

            // log
            if (enumClass->getIsLogConfigIndependent())
                customIncludeFiles.insert(L"log_config.hpp");
            
            // action
            if (enumClass->getIsActionManagerIndependent())
                customIncludeFiles.insert(L"action_manager.hpp");

            // thread
            if (enumClass->getIsThreadManagerIndependent())
                customIncludeFiles.insert(L"thread_manager.hpp");
        }
    }
    if (isContainJson) {
        systemIncludeFiles.insert(L"assert.h");
        systemIncludeFiles.insert(L"memory");
        systemIncludeFiles.insert(L"string");

        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_document.hpp");
        customIncludeFiles.insert(L"i_document_builder.hpp");
        customIncludeFiles.insert(L"json.hpp");
        customIncludeFiles.insert(L"number_helper.hpp");
        customIncludeFiles.insert(L"string_helper.hpp");
    
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L""))
                continue;
            for (auto const &property : enumClass->getProperties()) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Property)
                    continue;

                if (!property->getType1().empty() && vcc::isCapital(property->getType1())) {
                    if (classPathMapping.find(property->getType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->getType1()));
                    else if (classPathMapping.find(L"vcc::" + property->getType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->getType1()));
                }
                if (!property->getType2().empty() && vcc::isCapital(property->getType2())) {
                    if (classPathMapping.find(property->getType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->getType2()));
                    else if (classPathMapping.find(L"vcc::" + property->getType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->getType2()));
                }
            }
        }
    }
}

std::wstring VPGObjectFileGenerationService::getCppCustomHeader(const bool &isContainForm)
{
    std::wstring result = L"";
    TRY
        if (isContainForm)
            result += L"\r\n"
                + getVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                + getVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getCppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + className + L"::" + className + L"() : " + baseClassNameWithoutQuote + L"()\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"_ObjectType = ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L";\r\n"
                + getConstructorContent(enumClass, enumClassMapping, false)
                + INDENT + INDENT + L"initialize();\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getCppCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form)
            return getCloneFunction(enumClass, className, enumClassMapping, true);
    CATCH
    return L"";
}

std::wstring VPGObjectFileGenerationService::getCppJsonFunction(const std::wstring &className,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::shared_ptr<VPGEnumClass> enumClass)
{
    if (!enumClass->getIsJson())
        return L"";
    
    std::wstring content = L"";
    TRY
        std::wstring toJsonVarable = L"";
        std::wstring deserializeVariable = L"";
        //json
        std::wstring namingStyleStr = L"vcc::NamingStyle namestyle = vcc::NamingStyle::" + (enumClass->isJsonAttributesContainKey(L"Key.NamingStyle") ? enumClass->getJsonAttributesAtKey(L"Key.NamingStyle") : L"PascalCase") + L";";
        toJsonVarable += INDENT + INDENT + namingStyleStr + L"\r\n";
        deserializeVariable += INDENT + INDENT + namingStyleStr + L"\r\n";
        bool isHavingDecimal = false;
        for (auto const &property : enumClass->getProperties()) {
            if (property->getIsNoJson())
                continue;
            isHavingDecimal = (vcc::find(property->getMacro(), L"MAP") != std::wstring::npos && (property->getType2() == L"float" || property->getType2() == L"double"))
                || (vcc::find(property->getMacro(), L"MAP") == std::wstring::npos && (property->getType1() == L"float" || property->getType1() == L"double"));
            if (isHavingDecimal)
                break;
        }
        
        std::wstring decimalPlaces = (enumClass->isJsonAttributesContainKey(L"Value.DecimalPlaces") ? enumClass->getJsonAttributesAtKey(L"Value.DecimalPlaces") : L"-1");
        if (isHavingDecimal)
            toJsonVarable += INDENT + INDENT + L"int64_t decimalPlaces = " + decimalPlaces + L";\r\n";
        
        // content
        std::wstring toJsonStr = L"";
        std::wstring deserializeStr = L"";
        for (auto const &property : enumClass->getProperties()) {
            if (property->getPropertyType() != VPGEnumClassAttributeType::Property || property->getIsNoJson())
                continue;
                
            std::wstring propertyName = property->getPropertyName();
            std::wstring convertedPropertyName = vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, property->getPropertyName());
            std::wstring originalType = getTypeOrClassWithoutNamespace(property->getType1());
            std::wstring originalMacro = vcc::find(property->getMacro(), L"(") != std::wstring::npos ? property->getMacro().substr(0, vcc::find(property->getMacro(), L"(")) : L"";
            if (originalMacro.empty() || originalType.empty())
                continue;

            bool isToJsonRequired = true;
            bool isDeserializeRequired = true;
            switch (property->getMacroType())
            {
            case VPGEnumClassMacroType::Setcustom:
            case VPGEnumClassMacroType::SetcustomSptr:
                isToJsonRequired = false;
                isDeserializeRequired = true;
                break;
            case VPGEnumClassMacroType::Getcustom:
            case VPGEnumClassMacroType::GetcustomSptr:
                isToJsonRequired = true;
                isDeserializeRequired = false;
                break;
            default:
                break;
            }
            std::wstring prefix = INDENT + INDENT + L"// " + property->getPropertyName() + L"\r\n";
            if (isToJsonRequired)
                toJsonStr += prefix;
            if (isDeserializeRequired)
                deserializeStr += prefix;

            if (property->getIsVector() || property->getIsSet()) {
                // To Json
                toJsonStr += INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<vcc::Json>();\r\n"
                    + INDENT + INDENT + L"json->addArray(vcc::convertNamingStyle(L" + convertedPropertyName + L", vcc::NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : get" + propertyName + L"()) {\r\n";
                
                deserializeStr += INDENT + INDENT + L"clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->isContainKey(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase))) {\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &element : json->getArray(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase))) {\r\n";
                if (vcc::isContain(originalMacro, L"SPTR")) {
                    // Object
                    toJsonStr += INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->addArrayObject(element->ToJson());\r\n";

                    deserializeStr += INDENT + INDENT + INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<" + originalType + L">();\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->deserializeJson(element->getArrayElementObject());\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"insert" + propertyName + L"(tmp" + propertyName + L");\r\n";
                } else {
                    std::vector<std::wstring> jsonStrings = getObjectToJson(enumClassMapping, L"tmp" + propertyName, originalMacro, originalType, L"element", true, false);
                    for (auto const &str : jsonStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    std::vector<std::wstring> objectStrings = getJsonToObject(enumClassMapping, L"element", originalMacro, originalType, L"", propertyName, true, false);
                    for (auto const &str : objectStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                }
                toJsonStr += INDENT + INDENT + L"}\r\n";
                deserializeStr += INDENT + INDENT + INDENT + L"}\r\n"
                    + INDENT + INDENT + L"}\r\n";

            } else if (property->getIsMap() || property->getIsOrderedMap()) {
                toJsonStr += INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<vcc::Json>();\r\n"
                    + INDENT + INDENT + L"json->addObject(vcc::convertNamingStyle(L" + convertedPropertyName + L", vcc::NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : get" + propertyName + L"()) {\r\n";
                
                deserializeStr += INDENT + INDENT + L"clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->isContainKey(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {\r\n"
                    + INDENT + INDENT + INDENT + L"auto tmpObject = json->getObject(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase));\r\n"
                    + INDENT + INDENT + INDENT + L"auto tmpKeys = tmpObject->getKeys();\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &key : tmpKeys) {\r\n";
                    
                std::wstring toJsonStrKeyStr = L"";
                if (vcc::isCapital(originalType)) {
                    std::vector<std::wstring> objectToJsonEnumKeySwitchStrings = getObjectToJsonEnumSwitch(L"element.first", L"keyStr", originalType, enumClassMapping);
                    for (auto const &str : objectToJsonEnumKeySwitchStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    toJsonStrKeyStr = L"keyStr";

                    std::vector<std::wstring> jsonToObjectEnumkeySwtichStrings = getJsonToObjectEnumSwitch(L"key", true, originalType, enumClassMapping);
                    for (auto const &str : jsonToObjectEnumkeySwtichStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                } else if (originalType == L"wstring")
                    toJsonStrKeyStr = L"element.first";
                else
                    toJsonStrKeyStr = L"std::to_wstring(element.first)";
                if (vcc::isContain(originalMacro, L"SPTR")) {
                    // Object
                    toJsonStr += INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->addObject(" + toJsonStrKeyStr + L", element.second->ToJson());\r\n";
                
                    deserializeStr += INDENT + INDENT + INDENT + INDENT + L"if (tmpObject->getObject(key) != nullptr) {\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"auto tmpElementObject = std::make_shared<" + property->getType2() + L">();\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"tmpElementObject->deserializeJson(tmpObject->getObject(key));\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"insert" + propertyName + L"AtKey(" + getJsonToObjectMapKey(originalType) + L", tmpElementObject);\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"} else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"insert" + propertyName + L"AtKey(" + getJsonToObjectMapKey(originalType) + L", nullptr);\r\n";
                } else {
                    std::vector<std::wstring> jsonStrings = getObjectToJson(enumClassMapping, L"tmp" + propertyName, originalMacro, property->getType2(), toJsonStrKeyStr, false, true);
                    for (auto const &str : jsonStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    std::vector<std::wstring> objectStrings = getJsonToObject(enumClassMapping, L"tmpObject", originalMacro, property->getType2(), property->getType1(), propertyName, false, true);
                    for (auto const &str : objectStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                }
                toJsonStr += INDENT + INDENT + L"}\r\n";
                deserializeStr += INDENT + INDENT + INDENT + L"}\r\n"
                    + INDENT + INDENT + L"}\r\n";

            } else {
                // Object To Json
                if (isToJsonRequired) {
                    for (auto const &str : getObjectToJson(enumClassMapping, L"json", originalMacro, originalType, propertyName, false, false))
                        toJsonStr += INDENT + INDENT + str + L"\r\n";
                }
                if (isDeserializeRequired) {
                    // Json To Object
                    if (property->getIsObject()) {
                        // Object
                        if (property->getIsInitializeInClassConstructorNeeded())
                            deserializeStr += INDENT + INDENT + L"set" + propertyName + L"(std::make_shared<" + property->getType1() + L">(" + property->getDefaultValue() + L"));\r\n";
                        else if (!property->getIsCustom())
                            deserializeStr += INDENT + INDENT + L"set" + propertyName + L"(nullptr);\r\n";
                        
                        deserializeStr += INDENT + INDENT + L"if (json->isContainKey(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase)) && json->getObject(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase)) != nullptr) {\r\n"
                            + INDENT + INDENT + INDENT + L"auto tmpObject = std::make_shared<" + property->getType1() + L">();\r\n";
                    } else if (vcc::isCapital(originalType)) {
                        // Enum
                        deserializeStr += INDENT + INDENT + L"if (json->isContainKey(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase))) {\r\n";
                    } else {
                        deserializeStr += INDENT + INDENT + L"if (json->isContainKey(vcc::convertNamingStyle(L" + convertedPropertyName + L", namestyle, vcc::NamingStyle::PascalCase)))\r\n";
                    }
                    for (auto const &str : getJsonToObject(enumClassMapping, L"json", originalMacro, originalType, L"", propertyName, false, false))
                        deserializeStr += INDENT + INDENT + str + L"\r\n";
                    if (property->getIsObject())
                        deserializeStr += INDENT + INDENT + INDENT + L"set" + propertyName + L"(tmpObject);\r\n";
                    if (property->getIsObject() || (!vcc::isContain(originalMacro, L"SPTR") && vcc::isCapital(originalType)))
                        deserializeStr += INDENT + INDENT + L"}\r\n";
                }
            }
        }
        
        content += L"\r\n"
            "std::shared_ptr<vcc::Json> " + className + L"::ToJson() const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + (!toJsonStr.empty() ? toJsonVarable : L"")
            + INDENT + INDENT + L"auto json = std::make_unique<vcc::Json>();\r\n"
            + toJsonStr
            + INDENT + INDENT + L"return json;\r\n"
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return nullptr;\r\n"
            "}\r\n"
            "\r\n"
            "void " + className + L"::deserializeJson(std::shared_ptr<vcc::IDocument> document)\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + (!deserializeStr.empty() ? deserializeVariable : L"")
            + INDENT + INDENT + L"auto json = std::dynamic_pointer_cast<vcc::Json>(document);\r\n"
            + INDENT + INDENT + L"assert(json != nullptr);\r\n"
            + deserializeStr
            + INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
    return content;
}

std::wstring VPGObjectFileGenerationService::getCppInitialize(const VPGEnumClass *enumClass, const std::wstring &className, const std::wstring &baseClassName)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                "void " + className + L"::initializeComponents()\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + baseClassName + L"::initializeComponents();\r\n";
            if (enumClass->getIsLogConfigIndependent())
                result += INDENT + INDENT + L"_LogConfig = std::make_shared<vcc::LogConfig>();\r\n";
            else
                result += INDENT + INDENT + L"_LogConfig = nullptr;\r\n";
            if (enumClass->getIsActionManagerIndependent())
                result += INDENT + INDENT + L"_ActionManager = std::make_shared<ActionManager>(_LogConfig);\r\n";
            else
                result += INDENT + INDENT + L"_ActionManager = nullptr;\r\n";
            if (enumClass->getIsThreadManagerIndependent())
                result += INDENT + INDENT + L"_ThreadManager = std::make_shared<ThreadManager>(_LogConfig);\r\n";
            else
                result += INDENT + INDENT + L"_ThreadManager = nullptr;\r\n";

            // Custom Manager
            std::set<std::wstring> customManagers;
            for (auto const &property : enumClass->getProperties()) {
                if (property->getPropertyType() == VPGEnumClassAttributeType::Manager) {
                    if (vcc::isStartWith(property->getMacro(), L"MANAGER_SPTR_PARENT"))
                        customManagers.insert(L"_" + property->getPropertyName() + L" = nullptr;");
                    else
                        customManagers.insert(L"_" + property->getPropertyName() + L" = std::make_shared<" + property->getType1() + L">(" + property->getDefaultValue() + L");");
                }
            }

            if (!customManagers.empty()) {
                result += INDENT + INDENT +  L"// Custom Managers\r\n";
                for (auto const &customManager : customManagers)
                    result += INDENT + INDENT + customManager + L"\r\n";
            }
            result += INDENT + INDENT + L"onInitializeComponents();\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getCppAction(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->getType() == VPGEnumClassType::Form) {
            bool isContainArgument = false;
            for (auto &property : enumClass->getProperties()) {
                if (property->getPropertyType() == VPGEnumClassAttributeType::Action
                    && !property->getType1().empty() && vcc::isCapital(property->getType1())) {
                    isContainArgument = true;
                    break;
                }
            }

            result += L"\r\n"
                "std::shared_ptr<vcc::IResult> " + className + L"::doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> " + (isContainArgument ? L"argument" : L"/*argument*/") + L")\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"switch(static_cast<" + enumClass->getName() + L">(formProperty))\r\n"
                + INDENT + INDENT + L"{\r\n";
            for (auto const& property : enumClass->getProperties()) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Action)
                    continue;
                result += INDENT + INDENT + L"case " + enumClass->getName() + L"::" + property->getEnum() + L":\r\n"
                    + INDENT + INDENT + INDENT + L"return do" + property->getPropertyName() + L"(" + (!property->getType1().empty() ? (L"std::dynamic_pointer_cast<" + property->getType1() + L">(argument)") : L"") + L");\r\n";
            }
            result += INDENT + INDENT + L"default:\r\n"
                + INDENT + INDENT + INDENT + L"assert(false);\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n"
                + INDENT + INDENT + L"}\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return nullptr;\r\n"
                "}\r\n";

            for (auto const &property : enumClass->getProperties()) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Action || property->getIsInherit())
                    continue;
                
                std::wstring argumentList = L"";
                std::wstring assignmentStr = L"_LogConfig, sharedPtr()";
                if (!property->getType1().empty()) {
                    if (vcc::isCapital(property->getType1()))
                        argumentList += L"std::shared_ptr<" + property->getType1() + L"> argument";
                    else
                        argumentList = L"const " + property->getType1() + L" &argument";
                    assignmentStr += L", argument"; 
                }

                result += L"\r\n"
                    "std::shared_ptr<vcc::IResult> " + className + L"::do" + property->getPropertyName() + L"(" + argumentList + L")\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + L"auto action = std::make_shared<" + getActionClassName(enumClass, property.get()) + L">(" + assignmentStr + L");\r\n"
                    + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", className, L"Do" + property->getPropertyName()) + L"\r\n"
                    + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", className, L"Do" + property->getPropertyName()) + L"\r\n"
                    + INDENT + INDENT + L"return executeAction(action, " + (property->getIsNoHistory() ? L"true" : L"false") + L");\r\n"
                    + INDENT + L"CATCH\r\n"
                    + INDENT + L"return nullptr;\r\n"
                    "}\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::getCppCustomFunction(const bool &isContainForm)
{
    std::wstring result = L"";
    TRY
        if (isContainForm)
            result += L"\r\n"
                + getVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
                + getVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";
    CATCH
    return result;
}

void VPGObjectFileGenerationService::GenerateCpp(const vcc::LogConfig *logConfig,
    const std::wstring &classPrefix,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &objectFilePathCpp,
    const std::wstring &formFilePathCpp,
    const std::wstring &actionFolderPathCpp,
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        if (objectFilePathCpp.empty() && formFilePathCpp.empty())
            return;

        bool isIncludeJson = false;
        bool isIncludeForm = false;
        for (auto const &enumClass : enumClassList) {
            isIncludeJson |= enumClass->getIsJson();
            isIncludeForm |= enumClass->getType() == VPGEnumClassType::Form;
        }
        if (!isIncludeJson && !isIncludeForm)
            return;
        
        std::wstring filePathCpp = isIncludeForm && !formFilePathCpp.empty() ? formFilePathCpp : objectFilePathCpp;
        std::wstring includeFileName = vcc::getFileName(filePathCpp);
        vcc::replace(includeFileName, L".cpp", L".hpp");

        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object class file: " + filePathCpp);
        
        // ------------------------------------------------------------------------------------------ //
        //                               include Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> customIncludeFiles;
        getCppIncludeFiles(classPathMapping,
            enumClassList,
            isIncludeForm,
            isIncludeJson,
            systemIncludeFiles,
            customIncludeFiles);

        // ------------------------------------------------------------------------------------------ //
        //                               Action  Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::map<std::wstring, std::wstring> actionNameAndActionClassList;
        for (auto const &enumClass : enumClassList)
            VPGActionFileGenerationService::GenerateCpp(logConfig, classPathMapping, enumClass.get(), classPrefix, actionFolderPathCpp, actionNameAndActionClassList, systemIncludeFiles, customIncludeFiles);      

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Script                                              //
        // ------------------------------------------------------------------------------------------ //
        std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#include \"" + includeFileName + L"\"\r\n";

        if (!systemIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &str : systemIncludeFiles)
                content += L"#include <" + str + L">\r\n";
        }
        if (!customIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &str : customIncludeFiles) {
                if (str == includeFileName)
                    continue;
                content += L"#include \"" + str + L"\"\r\n";
            }
        }

        content += getCppCustomHeader(isIncludeForm);

        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        for (auto const &pair : actionNameAndActionClassList) {
            std::wstring currentNamespace = getNamespaceFromClassName(pair.first);
            if (!vcc::isContain(namespaceClassMapping, currentNamespace))
                namespaceClassMapping.insert({currentNamespace, {}});
            namespaceClassMapping[currentNamespace].push_back(pair.second);
        }
            
        // Generate Part
        for (auto const &enumClass : enumClassList) {
            if (!IsPropertyClass(enumClass->getName(), L""))
                continue;
                
            std::wstring className = getClassNameFromPropertyClassName(getTypeOrClassWithoutNamespace(enumClass->getName()));

            std::wstring baseClassName = (enumClass->getType() == VPGEnumClassType::Form ? L"vcc::BaseForm" : L"vcc::BaseObject");
            if (!vcc::isBlank(enumClass->getInheritClass()))
                baseClassName = enumClass->getInheritClass();                
            std::wstring baseClassNameWithoutQuote = baseClassName;
            if (vcc::isContain(baseClassNameWithoutQuote, L"<"))
                baseClassNameWithoutQuote = baseClassNameWithoutQuote.substr(0, vcc::find(baseClassNameWithoutQuote, L"<"));
                
            std::wstring currentNamespace = getNamespaceFromClassName(enumClass->getName());
            if (!vcc::isContain(namespaceClassMapping, currentNamespace))
                namespaceClassMapping.insert({currentNamespace, {}});
            namespaceClassMapping[currentNamespace].push_back(
                getCppConstructor(enumClass.get(), classPrefix, className, baseClassNameWithoutQuote, enumClassMapping)
                + getCppCloneFunctions(enumClass.get(), className, enumClassMapping)
                + getCppJsonFunction(className, enumClassMapping, enumClass)
                + getCppInitialize(enumClass.get(), className, baseClassNameWithoutQuote)
                + getCppAction(enumClass.get(), className));
        }

        // 4. Generate code in namespace
        content += generateCodeWithNamespace(namespaceClassMapping);

        // 5. Custom Functions
        content += getCppCustomFunction(isIncludeForm);
            
        // ------------------------------------------------------------------------------------------ //
        //                               Handle VCC Tag                                               //
        // ------------------------------------------------------------------------------------------ //
        if (vcc::isFilePresent(filePathCpp))
            content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
        
        vcc::lTrim(content);
        vcc::writeFile(filePathCpp, content, true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate object class file completed.");
    CATCH
}
