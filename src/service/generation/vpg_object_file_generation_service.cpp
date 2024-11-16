#include "vpg_object_file_generation_service.hpp"

#include <map>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "memory_macro.hpp"
#include "set_helper.hpp"

#include "vpg_enum_class.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_include_path_service.hpp"

using namespace vcc;

#define LOG_ID L"Object File Generation"
const std::wstring propertyClassNameSuffix = L"Property";

std::vector<std::wstring> VPGObjectFileGenerationService::GetObjectToJsonEnumSwitch(const std::wstring &switchVariable, const std::wstring &returnVariable,
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
        for (auto const &enumEnumClassProperty : enumEnumClass->GetProperties()) {
            result.push_back(L"case " + enumEnumClass->GetName() + L"::" + enumEnumClassProperty->GetEnum() + L":");
            result.push_back(INDENT + returnVariable + L" = L\"" + enumEnumClassProperty->GetEnum() + L"\";");
            result.push_back(INDENT + L"break;");
        }
        result.push_back(L"default:");
        result.push_back(INDENT + L"assert(false);");
        result.push_back(INDENT + L"break;");
        result.push_back(L"}");
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::GetJsonToObjectEnumSwitch(const std::wstring &propertyName, const bool isKey,
    const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    std::vector<std::wstring> result;
    TRY
        std::wstring jsonEnumValue = isKey ? L"keyEnum" : L"valueEnum";
        std::wstring jsonStr = jsonEnumValue + L"Str";
        std::wstring jsonStrUpper = jsonStr + L"Upper";

        result.push_back(INDENT + L"std::wstring " + jsonStr + L" = " + (isKey ? L"key;" : (L"json->GetString(ConvertNamingStyle(L" + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, propertyName) + L", namestyle, NamingStyle::PascalCase));")));
        result.push_back(INDENT + L"std::wstring " + jsonStrUpper + L" = " + jsonStr + L";");
        result.push_back(INDENT + L"ToUpper(" + jsonStrUpper + L");");
        result.push_back(INDENT + L"int64_t " + jsonEnumValue + L" = -1;");

        std::shared_ptr<VPGEnumClass> enumEnumClass = nullptr;
        if (enumClassMapping.find(type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(type);
        } else if (enumClassMapping.find(L"vcc::" + type) != enumClassMapping.end()) {
            enumEnumClass = enumClassMapping.at(L"vcc::" + type);
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"VPGObjectFileGenerationService::GenerateCpp Enum Class " + type + L" cannot found");
        
        bool isStart = true;
        for (auto const &enumEnumClassProperty : enumEnumClass->GetProperties()) {
            std::wstring ifPrefix = isStart ? L"" : L"else ";
            std::wstring enumNameUpper = enumEnumClassProperty->GetEnum();
            ToUpper(enumNameUpper);
            result.push_back(INDENT + ifPrefix + L"if (" + jsonStrUpper + L" == L\"" + enumNameUpper + L"\")");
            result.push_back(INDENT + INDENT + jsonEnumValue + L" = static_cast<int64_t>(" + enumEnumClass->GetName() + L"::" + enumEnumClassProperty->GetEnum() + L");");
            isStart = false;
        }
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::GetObjectToJson(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &propertyName, const bool &isArray, const bool &isMap)
{
    std::vector<std::wstring> result;
    TRY
        std::wstring arrayStr = isArray ? L"Array" : L"";
        std::wstring convertedPropertyName = isMap ? (propertyName + L", ") : (!isArray ? (L"ConvertNamingStyle(L" + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, propertyName) + L", NamingStyle::PascalCase, namestyle), ") : L"");
        std::wstring currentPropertyName = isMap ? L"element.second" : ((!isArray ? L"_" : L"") + propertyName);
        if (IsContain(macro, L"SPTR")) {
            // Object
            result.push_back(parentName + L"->Add" + arrayStr + L"Object(" + convertedPropertyName + currentPropertyName + L"->ToJson());");
        } else if (std::iswupper(type[0])) {
            // Enum
            std::wstring tmpPropertyName = L"";
            if (isMap) {
                tmpPropertyName = L"valueStr";
            } else {
                tmpPropertyName = propertyName.substr(0, 1);
                ToLower(tmpPropertyName);
                if (propertyName.size() > 1)
                    tmpPropertyName += propertyName.substr(1);
                tmpPropertyName += L"ValueStr";
            }
            std::vector<std::wstring> enumSwitchStr = GetObjectToJsonEnumSwitch(currentPropertyName, tmpPropertyName, type, enumClassMapping);
            result.insert(result.end(), enumSwitchStr.begin(), enumSwitchStr.end());
            result.push_back(parentName + L"->Add" + arrayStr + L"String(" + convertedPropertyName  + tmpPropertyName +L");");
        } else if (type == L"bool")
            result.push_back(parentName + L"->Add" + arrayStr + L"Bool(" + convertedPropertyName + currentPropertyName + L");");
        else if (IsContain(type, L"int")
            || IsContain(type, L"short")
            || IsContain(type, L"long")
            || type == L"size_t"
            || type == L"time_t")
            result.push_back(parentName + L"->Add" + arrayStr + L"Int(" + convertedPropertyName  + currentPropertyName + L");");
        else if (type == L"double" || type == L"float")
            result.push_back(parentName + L"->Add" + arrayStr + L"Double(" + convertedPropertyName + currentPropertyName + L", decimalPlaces >= 0 ? decimalPlaces : GetDecimalPlaces(" + currentPropertyName + L"));");
        else if (type == L"std::string")
            result.push_back(parentName + L"->Add" + arrayStr + L"String(" + convertedPropertyName + L"str2wstr(" + currentPropertyName + L"));");
        else if (type == L"std::wstring")
            result.push_back(parentName + L"->Add" + arrayStr + L"String(" + convertedPropertyName  + currentPropertyName + L");");
        else
            result.push_back(parentName + L"->Add" + arrayStr + L"String(" + convertedPropertyName + L"std::to_wstring(" + currentPropertyName + L"));");
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetJsonToObjectMapKey(const std::wstring &type)
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
        } else if (IsContain(type, L"int")
            || IsContain(type, L"short")
            || IsContain(type, L"long")
            || type == L"size_t"
            || type == L"time_t")
            result = L"std::stoi(key)";
        else if (type == L"float" || type == L"double") {
            result = L"std::stod(key)";
        } else if (type == L"std::string") {
            result = L"wstr2str(key)";
        } else if (type == L"std::wstring") {
            result = L"key";
        } else if (IsCaptial(type)) {
            result = L"static_cast<" + type + L">(keyEnum)";
        }
        else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + type);
    CATCH
    return result;
}

std::vector<std::wstring> VPGObjectFileGenerationService::GetJsonToObject(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &mapKeyType, const std::wstring &propertyName, const bool &isArray, const bool &isMap)
{
    std::vector<std::wstring> result;
    TRY
        bool ifCaseNeedKey = IsCaptial(mapKeyType);
        bool ifCaseNeedValue = !IsContain(macro, L"SPTR") && std::iswupper(type[0]); // for enum only
        std::wstring arrayStr = isArray ? L"Array" : L"";
        std::wstring convertedPropertyNameForGeneral = isMap ? (ifCaseNeedKey ? L"keyEnumStr" : L"key") : (isArray ? L"" : (L"ConvertNamingStyle(L" + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, propertyName) + L", namestyle, NamingStyle::PascalCase)"));
        std::wstring arrayElementStr = convertedPropertyNameForGeneral.empty() ? L"ArrayElement" : L"";
        std::wstring currentParentName = isMap ? L"tmpObject" : parentName;
        std::wstring currentPropertyName = L"_" + propertyName;
        std::wstring insertPrefix = (isMap || isArray) ? (L"Insert" + propertyName + L"(" + (isMap ? (GetJsonToObjectMapKey(mapKeyType) + L", ") : L"")) : (currentPropertyName + L" = ");
        std::wstring insertSuffix = (isMap || isArray) ? L");" : L";";
        std::wstring indentPrefix = INDENT;
        if (ifCaseNeedKey || ifCaseNeedValue) {
            if (ifCaseNeedValue) {
                std::vector<std::wstring> enumSwitchStr = GetJsonToObjectEnumSwitch(propertyName, false, type, enumClassMapping);
                result.insert(result.end(), enumSwitchStr.begin(), enumSwitchStr.end());
            }
            std::wstring ifContent = ifCaseNeedValue ? L" && " : L"";
            ifContent = ifCaseNeedKey ? (L"keyEnum > -1" + ifContent) : L"";
            ifContent += ifCaseNeedValue ? L"valueEnum > -1" : L"";
            result.push_back(INDENT + L"if (" + ifContent + L")");
            indentPrefix += INDENT;
        }
        if (IsContain(macro, L"SPTR")) {
            // Object
            result.push_back(indentPrefix + L"_" + propertyName + L"->DeserializeJson(" + currentParentName + L"->GetObject(ConvertNamingStyle(L" + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, propertyName) + L", namestyle, NamingStyle::PascalCase)));");
        } else if (std::iswupper(type[0])) {
            // Enum
            result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(valueEnum)" + insertSuffix);
        } else {
            if (type == L"bool")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->Get" + arrayElementStr + L"Bool(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (type == L"char")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->Get" + arrayElementStr + L"Char(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (type == L"wchar_t")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->Get" + arrayElementStr + L"Wchar(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else if (IsContain(type, L"int")
                || IsContain(type, L"short")
                || IsContain(type, L"long")
                || type == L"size_t"
                || type == L"time_t")
                result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(" + currentParentName + L"->Get" + arrayElementStr + L"Int64(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"double" || type == L"float")
                result.push_back(indentPrefix + insertPrefix + L"static_cast<" + type + L">(" + currentParentName + L"->Get" + arrayElementStr + L"Double(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"std::string")
                result.push_back(indentPrefix + insertPrefix + L"wstr2str(" + currentParentName + L"->Get" + arrayElementStr + L"String(" + convertedPropertyNameForGeneral + L"))" + insertSuffix);
            else if (type == L"std::wstring")
                result.push_back(indentPrefix + insertPrefix + currentParentName + L"->Get" + arrayElementStr + L"String(" + convertedPropertyNameForGeneral + L")" + insertSuffix);
            else
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"VPGObjectFileGenerationService::GenerateCpp Unknown type: " + type);
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className)
{
    TRY
        if (projectClassIncludeFiles.contains(className))
            return projectClassIncludeFiles.at(className);
        else if (projectClassIncludeFiles.contains(L"vcc::" + className))
            return projectClassIncludeFiles.at(L"vcc::" + className);
    CATCH
    return L"";
}

void VPGObjectFileGenerationService::GetHppIncludeFiles(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
    bool &isContainForm,
    std::set<std::wstring> &systemFileList,
    std::set<std::wstring> &projectFileList,
    std::set<std::wstring> &abstractClassList,
    std::set<std::wstring> &abstractEnumClassList,
    std::set<std::wstring> &classInCurrentFileList)
{ 
    // TODO: need to enable to check all systemn function
    // need to skip all code in command
    // Mac: all next tokens after class _LIBCPP_TEMPLATE_VIS under path /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1
    // Window: C:\msys64\mingw64\include\c++\12.2.0

    projectFileList.insert(L"class_macro.hpp");
    projectFileList.insert(L"object_type.hpp");

    for (auto const &enumClass : enumClassList) {
        if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
            continue;

        std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());
        classInCurrentFileList.insert(className);
        // ------------------------------------------------------------------------------------------ //
        //                               Class Level                                                  //
        // ------------------------------------------------------------------------------------------ //
        switch (enumClass->GetType())
        {
        case VPGEnumClassType::Object:
            projectFileList.insert(L"base_object.hpp");
            break;
        case VPGEnumClassType::Form:
            projectFileList.insert(L"base_form.hpp");
            isContainForm = true;
            break;
        default:
            break;
        }

        // Json
        if (enumClass->GetIsJson()) {
            projectFileList.insert(L"base_json_object.hpp");
            projectFileList.insert(L"json.hpp");
            projectFileList.insert(L"i_document.hpp");
        }
        if (!IsBlank(enumClass->GetInheritClass())) {
            std::wstring inheritClass = enumClass->GetInheritClass();
            // remove template
            size_t pos = Find(inheritClass, L"<");
            if (pos != std::wstring::npos) {
                inheritClass = inheritClass.substr(0, pos);
            }
            projectFileList.insert(GetProjectClassIncludeFile(projectClassIncludeFiles, inheritClass));
        }
        
        // ------------------------------------------------------------------------------------------ //
        //                               Property Level                                               //
        // ------------------------------------------------------------------------------------------ //
        for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
            // handle enum without macro case
            if (property->GetMacro().empty() || property->GetIsInherit())
                continue;

            std::wstring type = property->GetType1();
            if (std::iswupper(type[0])) {
                if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {

                    std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                    if (!includeFile.empty())
                        projectFileList.insert(includeFile);
                    else
                        abstractClassList.insert(type);
                } else {
                    std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                    if (!includeFile.empty())
                        projectFileList.insert(includeFile);
                    else
                        abstractEnumClassList.insert(type);
                }
            } else {
                // TODO: need to enable to check all systemn function
                // system type
                if (CountSubstring(type, L"string") > 0)
                    systemFileList.insert(L"string");
            }

            type = property->GetType2();
            if (!type.empty() && std::iswupper(type[0])) {
                if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {

                    std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                    if (!includeFile.empty())
                        projectFileList.insert(includeFile);
                    else
                        abstractClassList.insert(type);
                } else {
                    std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                    if (!includeFile.empty())
                        projectFileList.insert(includeFile);
                    else
                        abstractEnumClassList.insert(type);
                }
            } else {
                // only suuport string
                // system type
                if (CountSubstring(type, L"string") > 0)
                    systemFileList.insert(L"string");
            }
        }
    }
}

void VPGObjectFileGenerationService::GenerateHpp(const LogConfig *logConfig,
    const std::wstring &classPrefix,
    const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::wstring &objectFilePathHpp,
    const std::wstring &formFilePathHpp,
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
        GetHppIncludeFiles(projectClassIncludeFiles,
            enumClassList,
            isContainForm,
            systemFileList,
            projectFileList,
            abstractClassList,
            abstractEnumClassList,
            classInCurrentFileList);
        
        std::wstring filePathHpp = isContainForm && !formFilePathHpp.empty() ? formFilePathHpp : objectFilePathHpp;
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object class file: " + filePathHpp);

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Script                                              //
        // ------------------------------------------------------------------------------------------ //
        std::wstring systemFileListStr = L"";
        std::wstring projectFileListStr = L"";
        for (auto const &str : systemFileList)
            systemFileListStr += L"#include <" + str + L">\r\n";

        std::wstring fileHpp = GetFileName(filePathHpp);
        for (auto const &str : projectFileList) {
            if (str == fileHpp)
                continue;
            projectFileListStr +=  L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L"\r\n";
        }
        std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#pragma once\r\n";
        content += L"\r\n";
        content += !systemFileListStr.empty() ? (systemFileListStr + L"\r\n") : L"";
        content += !projectFileListStr.empty() ? (projectFileListStr + L"\r\n") : L"";
        
        if (isContainForm) {
            content += L"// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
                "// </vcc:customHeader>\r\n"
                "\r\n";
        }

        // as using base_object and macro, must have namespace vcc
        content += L"using namespace vcc;\r\n";
        // for those class that cannot be found in file list
        for (auto const &str : abstractClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            content +=  L"\r\nclass " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L";";
        }
        for (auto const &str : abstractEnumClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            content +=  L"\r\nenum class " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L";";
        }
            
        // generate class
        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
            std::wstring inheritClass = L"";
            std::wstring extraFunction = L"";
            // Form
            if (enumClass->GetType() == VPGEnumClassType::Form) {
                extraFunction += L"\r\n"
                    + INDENT + INDENT + L"// <vcc:custom sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
                    + INDENT + INDENT + L"// Initialize\r\n"
                    + INDENT + INDENT + L"void OnInitialize() const override;\r\n"
                    + INDENT + INDENT + L"// Close\r\n"
                    + INDENT + INDENT + L"bool IsClosable() const override;\r\n"
                    + INDENT + INDENT + L"// </vcc:custom>\r\n";
            }
            // Json
            if (enumClass->GetIsJson()) {
                inheritClass += L", public BaseJsonObject";
                extraFunction += L"\r\n"
                    + INDENT + INDENT + L"virtual std::shared_ptr<Json> ToJson() const override;\r\n"
                    + INDENT + INDENT + L"virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;\r\n";
            }

            content += L"\r\n";
            
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());
            std::wstring baseClassName = (enumClass->GetType() == VPGEnumClassType::Form ? L"BaseForm" : L"BaseObject");
            if (!IsBlank(enumClass->GetInheritClass()))
                baseClassName = enumClass->GetInheritClass();                
            std::wstring baseClassNameWithoutQuote = baseClassName;
            if (IsContain(baseClassNameWithoutQuote, L"<"))
                baseClassNameWithoutQuote = baseClassNameWithoutQuote.substr(0, Find(baseClassNameWithoutQuote, L"<"));

            content += L"class " + className + L" : public " + baseClassName + inheritClass + L"\r\n";
            content += L"{\r\n";
            // generate properties
            for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
                // handle enum without macro case
                // Not generate inherited properties
                if (property->GetMacro().empty() || property->GetIsInherit())
                    continue;
                content += INDENT + property->GetMacro() + L"\r\n";
            }
            content += L"\r\n";
            content += INDENT + L"public:\r\n";
            if (!IsBlank(enumClass->GetInheritClass())) {
                content += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"()\r\n"
                        + INDENT + INDENT + L"{\r\n"
                        + INDENT + INDENT + INDENT + L"_ObjectType = ObjectType::" +  className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L";\r\n"
                        + INDENT + INDENT + L"}\r\n";
            } else if (enumClass->GetType() == VPGEnumClassType::Form)
                content += INDENT + INDENT + className + L"(std::shared_ptr<LogConfig> logConfig = nullptr) : " + baseClassNameWithoutQuote + L"(logConfig, ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L") {}\r\n";
            else
                content += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"(ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L") {}\r\n";
            content += INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n";

            // Clone
            content += L"\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<IObject> Clone() const override\r\n"
                + INDENT + INDENT + L"{\r\n";
            std::wstring cloneContent = L"";
            for (auto const &property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                if ((!property->GetType1().empty() && std::iswupper(property->GetType1()[0])) 
                    || (!property->GetType2().empty() && std::iswupper(property->GetType2()[0]))) {
                    if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
                        cloneContent += INDENT + INDENT + INDENT + L"obj->Clone" + property->GetPropertyName() + L"(this->_" + property->GetPropertyName() + L");\r\n";
                    }
                }
            }
            if (!cloneContent.empty()) {
                content += INDENT + INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::make_shared<" + className + L">(*this);\r\n"
                    + cloneContent
                    + INDENT + INDENT + INDENT + L"return obj;\r\n";
            } else
                content += INDENT + INDENT + INDENT + L"return std::make_shared<" + className + L">(*this);\r\n";
            content += INDENT + INDENT + L"}\r\n";

            // extra function
            content += extraFunction
                + L"};\r\n";
        }
        
        // ------------------------------------------------------------------------------------------ //
        //                               Handle VCC Tag                                               //
        // ------------------------------------------------------------------------------------------ //
        if (IsFileExists(filePathHpp))
            content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, ReadFile(filePathHpp), VPGFileContentSyncMode::Full, L"//");
        
        LTrim(content);
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object class file completed.");
    CATCH
}

void VPGObjectFileGenerationService::GetCppIncludeFiles(
    const std::map<std::wstring, std::wstring> &classPathMapping, 
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
    const bool &isContainForm,
    const bool &isContainJson,
    std::set<std::wstring> &systemIncludeFiles,
    std::set<std::wstring> &customIncludeFiles)
{
    // if (isIncludeForm) {

    // }
    if (isContainJson) {
        systemIncludeFiles.insert(L"assert.h");
        systemIncludeFiles.insert(L"memory");
        systemIncludeFiles.insert(L"string");

        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_document.hpp");
        customIncludeFiles.insert(L"i_document_builder.hpp");
        customIncludeFiles.insert(L"json.hpp");
        customIncludeFiles.insert(L"memory_macro.hpp");
        customIncludeFiles.insert(L"number_helper.hpp");
        customIncludeFiles.insert(L"string_helper.hpp");
    
        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
            for (auto const &property : enumClass->GetProperties()) {
                if (!property->GetType1().empty() && std::iswupper(property->GetType1()[0])) {
                    if (classPathMapping.find(property->GetType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->GetType1()));
                    else if (classPathMapping.find(L"vcc::" + property->GetType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->GetType1()));
                }
                if (!property->GetType2().empty() && std::iswupper(property->GetType2()[0])) {
                    if (classPathMapping.find(property->GetType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->GetType2()));
                    else if (classPathMapping.find(L"vcc::" + property->GetType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->GetType2()));
                }
            }
        }
    }
}

void VPGObjectFileGenerationService::GenerateCpp(const LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::wstring &objectFilePathCpp,
    const std::wstring &formFilePathCpp,
    const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        if (objectFilePathCpp.empty() && formFilePathCpp.empty())
            return;

        bool isIncludeJson = false;
        bool isIncludeForm = false;
        for (auto const &enumClass : enumClassList) {
            isIncludeJson |= enumClass->GetIsJson();
            isIncludeForm |= enumClass->GetType() == VPGEnumClassType::Form;
        }
        if (!isIncludeJson && !isIncludeForm)
            return;
        
        std::wstring filePathCpp = isIncludeForm && !formFilePathCpp.empty() ? formFilePathCpp : objectFilePathCpp;
        std::wstring includeFileName = GetFileName(filePathCpp);
        Replace(includeFileName, L".cpp", L".hpp");

        LogService::LogInfo(logConfig, LOG_ID, L"Generate object class file: " + filePathCpp);
        
        bool isCustomNeeded = isIncludeForm;
        // ------------------------------------------------------------------------------------------ //
        //                               Include Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> customIncludeFiles;
        GetCppIncludeFiles(classPathMapping,
            enumClassList,
            isIncludeForm,
            isIncludeJson,
            systemIncludeFiles,
            customIncludeFiles);

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
        
        if (isCustomNeeded)
            content += L"\r\n"
                "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
                "// </vcc:customHeader>\r\n";
        
        content += L"\r\n"
            "using namespace vcc;\r\n";

        // Generate Part
        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
                
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());
        
            content += GetCppExternalFunctionJson(className, enumClassMapping, enumClass);
        }

        if (isCustomNeeded) {
            content += L"\r\n"
                "// <vcc:custom sync=\"RESERVE\" gen=\"RESERVE\">";

            // Custom Part
            for (auto const &enumClass : enumClassList) {
                if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                    continue;
                    
                std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());
                content += GetCppExternalFunctionForm(className, enumClassMapping, enumClass);
            }
            
            content += L"// </vcc:custom>\r\n";
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Handle VCC Tag                                               //
        // ------------------------------------------------------------------------------------------ //
        if (IsFileExists(filePathCpp))
            content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, ReadFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
        
        LTrim(content);
        WriteFile(filePathCpp, content, true);
        LogService::LogInfo(logConfig, LOG_ID, L"Generate object class file completed.");
    CATCH
}

std::wstring VPGObjectFileGenerationService::GetCppExternalFunctionForm(const std::wstring &className,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::shared_ptr<VPGEnumClass> enumClass)
{
    if (enumClass->GetType() != VPGEnumClassType::Form)
        return L"";
    
    std::wstring content = L"";
    TRY
        content += L"\r\n"
            "void " + className + L"::OnInitialize() const\r\n"
            "{\r\n"
            "}\r\n"
            "\r\n"
            "bool " + className + L"::IsClosable() const\r\n"
            "{\r\n"
            + INDENT + L"return true;\r\n"
            "}\r\n";
    CATCH
    return content;
}

std::wstring VPGObjectFileGenerationService::GetCppExternalFunctionJson(const std::wstring &className,
    const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
    const std::shared_ptr<VPGEnumClass> enumClass)
{
    if (!enumClass->GetIsJson())
        return L"";
    
    std::wstring content = L"";
    TRY
        std::wstring toJsonVarable = L"";
        std::wstring deserializeVariable = L"";
        //json
        std::wstring namingStyleStr = L"NamingStyle namestyle = NamingStyle::" + (enumClass->IsJsonAttributesContainKey(L"Key.NamingStyle") ? enumClass->GetJsonAttributes(L"Key.NamingStyle") : L"PascalCase") + L";";
        toJsonVarable += INDENT + INDENT + namingStyleStr + L"\r\n";
        deserializeVariable += INDENT + INDENT + namingStyleStr + L"\r\n";
        bool isHavingDecimal = false;
        for (auto const &property : enumClass->GetProperties()) {
            isHavingDecimal = (Find(property->GetMacro(), L"MAP") != std::wstring::npos && (property->GetType2() == L"float" || property->GetType2() == L"double"))
                || (Find(property->GetMacro(), L"MAP") == std::wstring::npos && (property->GetType1() == L"float" || property->GetType1() == L"double"));
            if (isHavingDecimal)
                break;
        }
        
        std::wstring decimalPlaces = (enumClass->IsJsonAttributesContainKey(L"Value.DecimalPlaces") ? enumClass->GetJsonAttributes(L"Value.DecimalPlaces") : L"-1");
        if (isHavingDecimal)
            toJsonVarable += INDENT + INDENT + L"int64_t decimalPlaces = " + decimalPlaces + L";\r\n";
        
        // content
        std::wstring toJsonStr = L"";
        std::wstring deserializeStr = L"";
        for (auto const &property : enumClass->GetProperties()) {
            std::wstring propertyName = property->GetPropertyName();
            std::wstring convertedPropertyName = GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, property->GetPropertyName());
            std::wstring originalType = property->GetType1();
            std::wstring originalMacro = Find(property->GetMacro(), L"(") != std::wstring::npos ? property->GetMacro().substr(0, Find(property->GetMacro(), L"(")) : L"";
            if (originalMacro.empty() || originalType.empty())
                continue;

            std::wstring prefix = INDENT + INDENT + L"// " + property->GetPropertyName() + L"\r\n";
            toJsonStr += prefix;
            deserializeStr += prefix;

            if (IsStartWith(originalMacro, L"VECTOR")
                || IsStartWith(originalMacro, L"SET")) {
                // To Json
                toJsonStr += INDENT + INDENT + L"DECLARE_SPTR(Json, tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"json->AddArray(ConvertNamingStyle(L" + convertedPropertyName + L", NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : _" + propertyName + L") {\r\n";
                
                deserializeStr += INDENT + INDENT + L"Clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &element : json->GetArray(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n";
                if (IsContain(originalMacro, L"SPTR")) {
                    // Object
                    toJsonStr += INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->AddArrayObject(element->ToJson());\r\n";

                    deserializeStr += INDENT + INDENT + INDENT + INDENT + L"DECLARE_SPTR(" + originalType + L", tmp" + propertyName + L");\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->DeserializeJson(element->GetArrayElementObject());\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"Insert" + propertyName + L"(tmp" + propertyName + L");\r\n";
                } else {
                    std::vector<std::wstring> jsonStrings = GetObjectToJson(enumClassMapping, L"tmp" + propertyName, originalMacro, originalType, L"element", true, false);
                    for (auto const &str : jsonStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    std::vector<std::wstring> objectStrings = GetJsonToObject(enumClassMapping, L"element", originalMacro, originalType, L"", propertyName, true, false);
                    for (auto const &str : objectStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                }
                toJsonStr += INDENT + INDENT + L"}\r\n";
                deserializeStr += INDENT + INDENT + INDENT + L"}\r\n"
                    + INDENT + INDENT + L"}\r\n";

            } else if (IsStartWith(originalMacro, L"MAP") || IsStartWith(originalMacro, L"ORDERED_MAP")) {
                toJsonStr += INDENT + INDENT + L"DECLARE_SPTR(Json, tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"json->AddObject(ConvertNamingStyle(L" + convertedPropertyName + L", NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : _" + propertyName + L") {\r\n";
                
                deserializeStr += INDENT + INDENT + L"Clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) != nullptr) {\r\n"
                    + INDENT + INDENT + INDENT + L"std::shared_ptr<Json> tmpObject = json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase));\r\n"
                    + INDENT + INDENT + INDENT + L"std::vector<std::wstring> tmpKeys = tmpObject->GetKeys();\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &key : tmpKeys) {\r\n";
                    
                std::wstring toJsonStrKeyStr = L"";
                if (IsCaptial(originalType)) {
                    std::vector<std::wstring> objectToJsonEnumKeySwitchStrings = GetObjectToJsonEnumSwitch(L"element.first", L"keyStr", originalType, enumClassMapping);
                    for (auto const &str : objectToJsonEnumKeySwitchStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    toJsonStrKeyStr = L"keyStr";

                    std::vector<std::wstring> jsonToObjectEnumkeySwtichStrings = GetJsonToObjectEnumSwitch(L"key", true, originalType, enumClassMapping);
                    for (auto const &str : jsonToObjectEnumkeySwtichStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                } else if (originalType == L"std::wstring")
                    toJsonStrKeyStr = L"element.first";
                else
                    toJsonStrKeyStr = L"std::to_wstring(element.first)";
                if (IsContain(originalMacro, L"SPTR")) {
                    // Object
                    toJsonStr += INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->AddObject(" + toJsonStrKeyStr + L", element.second->ToJson());\r\n";
                
                    deserializeStr += INDENT + INDENT + INDENT + INDENT + L"if (tmpObject->GetObject(key) != nullptr) {\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"DECLARE_SPTR(" + property->GetType2() + L", tmpElementObject);\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"tmpElementObject->DeserializeJson(tmpObject->GetObject(key));\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"Insert" + propertyName + L"(" + GetJsonToObjectMapKey(originalType) + L", tmpElementObject);\r\n"
                        + INDENT + INDENT + INDENT + INDENT + L"} else\r\n"
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"Insert" + propertyName + L"(" + GetJsonToObjectMapKey(originalType) + L", nullptr);\r\n";
                } else {
                    std::vector<std::wstring> jsonStrings = GetObjectToJson(enumClassMapping, L"tmp" + propertyName, originalMacro, property->GetType2(), toJsonStrKeyStr, false, true);
                    for (auto const &str : jsonStrings)
                        toJsonStr += INDENT + INDENT + INDENT + str + L"\r\n";

                    std::vector<std::wstring> objectStrings = GetJsonToObject(enumClassMapping, L"tmpObject", originalMacro, property->GetType2(), property->GetType1(), propertyName, false, true);
                    for (auto const &str : objectStrings)
                        deserializeStr += INDENT + INDENT + INDENT + str + L"\r\n";
                }
                toJsonStr += INDENT + INDENT + L"}\r\n";
                deserializeStr += INDENT + INDENT + INDENT + L"}\r\n"
                    + INDENT + INDENT + L"}\r\n";

            } else {
                // Object To Json
                for (auto const &str : GetObjectToJson(enumClassMapping, L"json", originalMacro, originalType, propertyName, false, false))
                    toJsonStr += INDENT + INDENT + str + L"\r\n";

                // Json To Object
                if (IsContain(originalMacro, L"SPTR")) {
                    // Object
                    deserializeStr += INDENT + INDENT + L"_" + propertyName + L" = nullptr;\r\n"
                        + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) != nullptr)\r\n";
                } else if (IsCaptial(originalType)) {
                    // Enum
                    deserializeStr += INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n";
                } else {
                    deserializeStr += INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)))\r\n";
                }
                for (auto const &str : GetJsonToObject(enumClassMapping, L"json", originalMacro, originalType, L"", propertyName, false, false))
                    deserializeStr += INDENT + INDENT + str + L"\r\n";
                if (!IsContain(originalMacro, L"SPTR") && std::iswupper(originalType[0]))
                    deserializeStr += INDENT + INDENT + L"}\r\n";
            }
        }
        
        content += L"\r\n"
            "std::shared_ptr<Json> " + className + L"::ToJson() const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + toJsonVarable
            + INDENT + INDENT + L"DECLARE_UPTR(Json, json);\r\n"
            + toJsonStr
            + INDENT + INDENT + L"return json;\r\n"
            + INDENT + L"CATCH\r\n"
            + INDENT + L"return nullptr;\r\n"
            "}\r\n"
            "\r\n"
            "void " + className + L"::DeserializeJson(std::shared_ptr<IDocument> document) const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + deserializeVariable
            + INDENT + INDENT + L"std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);\r\n"
            + INDENT + INDENT + L"assert(json != nullptr);\r\n"
            + deserializeStr
            + INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
    return content;
}