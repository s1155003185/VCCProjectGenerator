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
#include "vpg_enum_class.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_tag_helper.hpp"

using namespace vcc;

#define LOG_ID L"Object File Generation"
const std::wstring propertyClassNameSuffix = L"Property";

std::wstring VPGObjectFileGenerationService::GetCloneFunction(const VPGEnumClass *enumClass, const std::wstring &className, const bool &isCpp)
{
    std::wstring result = L"";
    TRY
        bool isContentNeeded = false;
        result += L"\r\n";
        std::wstring indent = !isCpp ? (INDENT + INDENT) : L"";

        if (enumClass->GetType() == VPGEnumClassType::Form) {
            if (isCpp) {
                result += indent + L"std::shared_ptr<IObject> " + className + L"::Clone() const\r\n";
                isContentNeeded = true;
            } else {
                result += indent + L"virtual std::shared_ptr<IObject> Clone() const override;\r\n";
                isContentNeeded = false;
            }
        } else {
            result += indent + L"virtual std::shared_ptr<IObject> Clone() const override\r\n";
            isContentNeeded = true;
        }

        if (isContentNeeded) {
            result += indent + L"{\r\n";
            std::wstring cloneContent = L"";
            for (auto const &property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;
                if ((!property->GetType1().empty() && IsCapital(property->GetType1())) 
                    || (!property->GetType2().empty() && IsCapital(property->GetType2()))) {
                    if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
                        cloneContent += indent + INDENT + L"obj->Clone" + property->GetPropertyName() + L"(this->_" + property->GetPropertyName() + (IsStartWith(property->GetMacro(), L"GETSET") ? L".get()" : L"" ) + L");\r\n";
                    }
                }
            }
            if (!cloneContent.empty()) {
                result += indent + INDENT + L"auto obj = std::make_shared<" + className + L">(*this);\r\n"
                    + cloneContent
                    + indent + INDENT + L"return obj;\r\n";
            } else
                result += indent + INDENT + L"return std::make_shared<" + className + L">(*this);\r\n";
            result += indent + L"}\r\n";
        }
    CATCH
    return result;
}

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
        } else if (IsCapital(type)) {
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
        } else if (IsCapital(type)) {
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
        bool ifCaseNeedKey = IsCapital(mapKeyType);
        bool ifCaseNeedValue = !IsContain(macro, L"SPTR") && IsCapital(type); // for enum only
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
        } else if (IsCapital(type)) {
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
        if (IsContain(projectClassIncludeFiles, className))
            return projectClassIncludeFiles.at(className);
        else if (IsContain(projectClassIncludeFiles, L"vcc::" + className))
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
            if (property->GetPropertyType() == VPGEnumClassPropertyType::NA || property->GetPropertyType() == VPGEnumClassPropertyType::Action || property->GetIsInherit())
                continue;

            std::wstring type = property->GetType1();
            if (IsCapital(type)) {
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
            if (!type.empty() && IsCapital(type)) {
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
            
            if (property->GetPropertyType() == VPGEnumClassPropertyType::Manager && IsStartWith(property->GetMacro(), L"MANAGER_SPTR_PARENT")) {
                type = property->GetDefaultValue();
                if (!type.empty() && IsCapital(type)) {
                    std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                    if (!includeFile.empty())
                        projectFileList.insert(includeFile);
                    else
                        abstractClassList.insert(type);
                }
            }
        }
    }
}

std::wstring VPGObjectFileGenerationService::GetHppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote)
{
    std::wstring result = L"";
    TRY
        // Constructor
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += INDENT + INDENT + className + L"();\r\n";
        } else {
            if (!IsBlank(enumClass->GetInheritClass()))
                result += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"()\r\n"
                    + INDENT + INDENT + L"{\r\n"
                    + INDENT + INDENT + INDENT + L"_ObjectType = ObjectType::" +  className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L";\r\n"
                    + INDENT + INDENT + L"}\r\n";
            else
                result += INDENT + INDENT + className + L"() : " + baseClassNameWithoutQuote + L"(ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L") {}\r\n";
        }
        // Destructor
        result += INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n";

    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppProperties(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        // generate properties
        for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
            // handle enum without macro case
            // Not generate inherited properties
            if (property->GetMacro().empty() || property->GetIsInherit())
                continue;
            result += INDENT + property->GetMacro() + L"\r\n";
        }

        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + GetVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + GetVccTagTailerCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppPrivateFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + L"private:\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPrivateFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassPrivateFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppProtectedFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + L"protected:\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppPublicCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        result = GetCloneFunction(enumClass, className, false);
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppPublicJsonFunctions(const VPGEnumClass *enumClass)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetIsJson())
            result += L"\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<Json> ToJson() const override;\r\n"
                + INDENT + INDENT + L"virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppPublicFunctions(const VPGEnumClass *enumClass)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + INDENT + L"virtual void InitializeComponents() const override;\r\n"
                "\r\n"
                + INDENT + INDENT + L"virtual void DoAction(const int64_t &formProperty) override;\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetHppPublicCustomFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPublicFunctions(VPGCodeType::Cpp, className) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassPublicFunctions(VPGCodeType::Cpp, className) + L"\r\n";
        }
    CATCH
    return result;    
}

void VPGObjectFileGenerationService::GenerateHpp(const LogConfig *logConfig,
    const std::wstring &classPrefix,
    const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
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
        //                               Action  Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::vector<std::wstring> actionClassList;
        for (auto const &enumClass : enumClassList)
            VPGActionFileGenerationService::GenerateHpp(logConfig, projectClassIncludeFiles, enumClass.get(), classPrefix, actionFolderPathHpp, actionClassList, systemFileList, projectFileList);        

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
            content += GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                "\r\n";
        }

        // as using base_object and macro, must have namespace vcc
        content += L"using namespace vcc;\r\n";
        // for those class that cannot be found in file list
        for (auto const &str : abstractClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            content +=  L"\r\n"
                "class " + str + L";";
        }
        for (auto const &str : abstractEnumClassList) {
            if (classInCurrentFileList.find(str) != classInCurrentFileList.end())
                continue;
            content +=  L"\r\n"
                "enum class " + str + L";";
        }

        for (auto const &str : actionClassList)
            content += L"\r\n"
                + str;
            
        // generate class
        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());
            
            std::wstring inheritClass = L"";
            // Json
            if (enumClass->GetIsJson())
                inheritClass += L", public BaseJsonObject";

            std::wstring baseClassName = (enumClass->GetType() == VPGEnumClassType::Form ? L"BaseForm" : L"BaseObject");
            if (!IsBlank(enumClass->GetInheritClass()))
                baseClassName = enumClass->GetInheritClass();                
            std::wstring baseClassNameWithoutQuote = baseClassName;
            if (IsContain(baseClassNameWithoutQuote, L"<"))
                baseClassNameWithoutQuote = baseClassNameWithoutQuote.substr(0, Find(baseClassNameWithoutQuote, L"<"));

            content += L"\r\n"
                "class " + className + L" : public " + baseClassName + inheritClass + L"\r\n"
                "{\r\n"
                + GetHppProperties(enumClass.get(), className)
                + GetHppPrivateFunctions(enumClass.get(), className)
                + GetHppProtectedFunctions(enumClass.get(), className)
                + L"\r\n"
                + INDENT + L"public:\r\n"
                + GetHppConstructor(enumClass.get(), classPrefix, className, baseClassNameWithoutQuote)
                + GetHppPublicCloneFunctions(enumClass.get(), className)
                + GetHppPublicJsonFunctions(enumClass.get())
                + GetHppPublicFunctions(enumClass.get())
                + GetHppPublicCustomFunctions(enumClass.get(), className)
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
    if (isContainForm) {
        systemIncludeFiles.insert(L"assert.h");
        systemIncludeFiles.insert(L"memory");
        
        customIncludeFiles.insert(L"exception_macro.hpp");

        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;

            // Property Enum Class
            customIncludeFiles.insert(GetProjectClassIncludeFile(classPathMapping, enumClass->GetName()));

            // Inherit Form
            if (!IsBlank(enumClass->GetInheritClass())) {
                std::wstring inheritClass = enumClass->GetInheritClass();
                // remove template
                size_t pos = Find(inheritClass, L"<");
                if (pos != std::wstring::npos) {
                    inheritClass = inheritClass.substr(0, pos);
                }
                customIncludeFiles.insert(GetProjectClassIncludeFile(classPathMapping, inheritClass));
            } else {
                customIncludeFiles.insert(L"base_form.hpp");
            }

            // log
            if (enumClass->GetIsLogConfigIndependent())
                customIncludeFiles.insert(L"log_config.hpp");
            
            // action
            if (enumClass->GetIsActionManagerIndependent())
                customIncludeFiles.insert(L"action_manager.hpp");

            // thread
            if (enumClass->GetIsThreadManagerIndependent())
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
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
            for (auto const &property : enumClass->GetProperties()) {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;

                if (!property->GetType1().empty() && IsCapital(property->GetType1())) {
                    if (classPathMapping.find(property->GetType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->GetType1()));
                    else if (classPathMapping.find(L"vcc::" + property->GetType1()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->GetType1()));
                }
                if (!property->GetType2().empty() && IsCapital(property->GetType2())) {
                    if (classPathMapping.find(property->GetType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(property->GetType2()));
                    else if (classPathMapping.find(L"vcc::" + property->GetType2()) != classPathMapping.end())
                        customIncludeFiles.insert(classPathMapping.at(L"vcc::" + property->GetType2()));
                }
            }
        }
    }
}

std::wstring VPGObjectFileGenerationService::GetCppCustomHeader(const bool &isContainForm)
{
    std::wstring result = L"";
    TRY
        if (isContainForm)
            result += L"\r\n"
                + GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetCppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form)
            result += L"\r\n"
                + className + L"::" + className + L"() : " + baseClassNameWithoutQuote + L"()\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"_ObjectType = ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L";\r\n"
                + INDENT + INDENT + L"Initialize();\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetCppCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form)
            result += GetCloneFunction(enumClass, className, true);
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetCppJsonFunction(const std::wstring &className,
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
        std::wstring namingStyleStr = L"NamingStyle namestyle = NamingStyle::" + (enumClass->IsJsonAttributesContainKey(L"Key.NamingStyle") ? enumClass->GetJsonAttributesAtKey(L"Key.NamingStyle") : L"PascalCase") + L";";
        toJsonVarable += INDENT + INDENT + namingStyleStr + L"\r\n";
        deserializeVariable += INDENT + INDENT + namingStyleStr + L"\r\n";
        bool isHavingDecimal = false;
        for (auto const &property : enumClass->GetProperties()) {
            isHavingDecimal = (Find(property->GetMacro(), L"MAP") != std::wstring::npos && (property->GetType2() == L"float" || property->GetType2() == L"double"))
                || (Find(property->GetMacro(), L"MAP") == std::wstring::npos && (property->GetType1() == L"float" || property->GetType1() == L"double"));
            if (isHavingDecimal)
                break;
        }
        
        std::wstring decimalPlaces = (enumClass->IsJsonAttributesContainKey(L"Value.DecimalPlaces") ? enumClass->GetJsonAttributesAtKey(L"Value.DecimalPlaces") : L"-1");
        if (isHavingDecimal)
            toJsonVarable += INDENT + INDENT + L"int64_t decimalPlaces = " + decimalPlaces + L";\r\n";
        
        // content
        std::wstring toJsonStr = L"";
        std::wstring deserializeStr = L"";
        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                continue;
                
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
                toJsonStr += INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<Json>();\r\n"
                    + INDENT + INDENT + L"json->AddArray(ConvertNamingStyle(L" + convertedPropertyName + L", NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : _" + propertyName + L") {\r\n";
                
                deserializeStr += INDENT + INDENT + L"Clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &element : json->GetArray(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n";
                if (IsContain(originalMacro, L"SPTR")) {
                    // Object
                    toJsonStr += INDENT + INDENT + INDENT + L"tmp" + propertyName + L"->AddArrayObject(element->ToJson());\r\n";

                    deserializeStr += INDENT + INDENT + INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<" + originalType + L">();\r\n"
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
                toJsonStr += INDENT + INDENT + L"auto tmp" + propertyName + L" = std::make_shared<Json>();\r\n"
                    + INDENT + INDENT + L"json->AddObject(ConvertNamingStyle(L" + convertedPropertyName + L", NamingStyle::PascalCase, namestyle), tmp" + propertyName + L");\r\n"
                    + INDENT + INDENT + L"for (auto const &element : _" + propertyName + L") {\r\n";
                
                deserializeStr += INDENT + INDENT + L"Clear" + propertyName + L"();\r\n"
                    + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) != nullptr) {\r\n"
                    + INDENT + INDENT + INDENT + L"auto tmpObject = json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase));\r\n"
                    + INDENT + INDENT + INDENT + L"auto tmpKeys = tmpObject->GetKeys();\r\n"
                    + INDENT + INDENT + INDENT + L"for (auto const &key : tmpKeys) {\r\n";
                    
                std::wstring toJsonStrKeyStr = L"";
                if (IsCapital(originalType)) {
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
                        + INDENT + INDENT + INDENT + INDENT + INDENT + L"auto tmpElementObject = std::make_shared<" + property->GetType2() + L">();\r\n"
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
                    deserializeStr += INDENT + INDENT + L"_" + propertyName + L" = std::make_shared<" + property->GetType1() + L">();\r\n"
                        + INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) && json->GetObject(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)) != nullptr)\r\n";
                } else if (IsCapital(originalType)) {
                    // Enum
                    deserializeStr += INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase))) {\r\n";
                } else {
                    deserializeStr += INDENT + INDENT + L"if (json->IsContainKey(ConvertNamingStyle(L" + convertedPropertyName + L", namestyle, NamingStyle::PascalCase)))\r\n";
                }
                for (auto const &str : GetJsonToObject(enumClassMapping, L"json", originalMacro, originalType, L"", propertyName, false, false))
                    deserializeStr += INDENT + INDENT + str + L"\r\n";
                if (!IsContain(originalMacro, L"SPTR") && IsCapital(originalType))
                    deserializeStr += INDENT + INDENT + L"}\r\n";
            }
        }
        
        content += L"\r\n"
            "std::shared_ptr<Json> " + className + L"::ToJson() const\r\n"
            "{\r\n"
            + INDENT + L"TRY\r\n"
            + toJsonVarable
            + INDENT + INDENT + L"auto json = std::make_unique<Json>();\r\n"
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
            + INDENT + INDENT + L"auto json = std::dynamic_pointer_cast<Json>(document);\r\n"
            + INDENT + INDENT + L"assert(json != nullptr);\r\n"
            + deserializeStr
            + INDENT + L"CATCH\r\n"
            "}\r\n";
    CATCH
    return content;
}

std::wstring VPGObjectFileGenerationService::GetCppInitialize(const VPGEnumClass *enumClass, const std::wstring &className, const std::wstring &baseClassName)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                "void " + className + L"::InitializeComponents() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + baseClassName + L"::InitializeComponents();\r\n";
            if (enumClass->GetIsLogConfigIndependent())
                result += INDENT + INDENT + L"_LogConfig = std::make_shared<LogConfig>();\r\n";
            else
                result += INDENT + INDENT + L"_LogConfig = nullptr;\r\n";
            if (enumClass->GetIsActionManagerIndependent())
                result += INDENT + INDENT + L"_ActionManager = std::make_shared<ActionManager>(_LogConfig);\r\n";
            else
                result += INDENT + INDENT + L"_ActionManager = nullptr;\r\n";
            if (enumClass->GetIsThreadManagerIndependent())
                result += INDENT + INDENT + L"_ThreadManager = std::make_shared<ThreadManager>(_LogConfig);\r\n";
            else
                result += INDENT + INDENT + L"_ThreadManager = nullptr;\r\n";

            // Custom Manager
            std::set<std::wstring> customManagers;
            for (auto const &property : enumClass->GetProperties()) {
                if (property->GetPropertyType() == VPGEnumClassPropertyType::Manager) {
                    if (IsStartWith(property->GetMacro(), L"MANAGER_SPTR_PARENT"))
                        customManagers.insert(L"_" + property->GetPropertyName() + L" = nullptr;");
                    else
                        customManagers.insert(L"_" + property->GetPropertyName() + L" = std::make_shared<" + property->GetType1() + L">(" + property->GetDefaultValue() + L");");
                }
            }

            if (!customManagers.empty()) {
                result += INDENT + INDENT +  L"// Custom Managers\r\n";
                for (auto const &customManager : customManagers)
                    result += INDENT + INDENT + customManager + L"\r\n";
            }
            result += INDENT + INDENT + L"OnInitializeComponents();\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetCppAction(const VPGEnumClass *enumClass, const std::wstring &className)
{
    std::wstring result = L"";
    TRY
        if (enumClass->GetType() == VPGEnumClassType::Form) {
            result += L"\r\n"
                "void " + className + L"::DoAction(const int64_t &formProperty)\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"switch(static_cast<" + enumClass->GetName() + L">(formProperty))\r\n"
                + INDENT + INDENT + L"{\r\n";
            for (auto const& property : enumClass->GetProperties()) {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Action)
                    continue;
                result += INDENT + INDENT + L"case " + enumClass->GetName() + L"::" + property->GetEnum() + L":\r\n"
                    + INDENT + INDENT + INDENT + L"Do" + property->GetPropertyName() + L"();\r\n"
                    + INDENT + INDENT + INDENT + L"break;\r\n";
            }
            result += INDENT + INDENT + L"default:\r\n"
                + INDENT + INDENT + INDENT + L"assert(false);\r\n"
                + INDENT + INDENT + INDENT + L"break;\r\n"
                + INDENT + INDENT + L"}\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";

            for (auto const &property : enumClass->GetProperties()) {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Action || property->GetIsInherit())
                    continue;
                
                std::wstring assignmentStr = L"_LogConfig, GetSharedPtr()";
                if (!property->GetClassAssignments().empty()) {
                    if (property->GetClassProperties().size() != property->GetClassAssignments().size())
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Enum Class " + enumClass->GetName() + L" Properties and Assignments Attribute @@Class ");

                    for (size_t i = 0; i < property->GetClassAssignments().size(); i++) {
                        std::wstring assignment = property->GetClassAssignmentsAtIndex(i);
                        if (IsStartWith(property->GetClassPropertiesAtIndex(i)->GetMacro(), L"GETSET")) {
                            if (Equal(property->GetClassPropertiesAtIndex(i)->GetType1(), L"std::wstring"))
                                assignment = L"L" + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, assignment);
                            else if (Equal(property->GetClassPropertiesAtIndex(i)->GetType1(), L"std::string"))
                                assignment = GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, assignment);
                        }
                        assignmentStr += L", " + assignment;
                    }
                }

                std::wstring functionName = L"Do" + property->GetPropertyName();
                result += L"\r\n"
                    "void " + className + L"::" + functionName + L"()\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + L"auto action = std::make_shared<" + GetActionClassName(enumClass, property.get()) + L">(" + assignmentStr + L");\r\n"
                    + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", className, functionName) + L"\r\n"
                    + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", className, functionName) + L"\r\n"
                    + INDENT + INDENT + L"ExecuteAction(action, " + (property->GetIsNoHistory() ? L"true" : L"false") + L");\r\n"
                    + INDENT + L"CATCH\r\n"
                    "}\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGObjectFileGenerationService::GetCppCustomFunction(const bool &isContainForm)
{
    std::wstring result = L"";
    TRY
        if (isContainForm)
            result += L"\r\n"
                + GetVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
                + GetVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";
    CATCH
    return result;
}

void VPGObjectFileGenerationService::GenerateCpp(const LogConfig *logConfig,
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
            isIncludeJson |= enumClass->GetIsJson();
            isIncludeForm |= enumClass->GetType() == VPGEnumClassType::Form;
        }
        if (!isIncludeJson && !isIncludeForm)
            return;
        
        std::wstring filePathCpp = isIncludeForm && !formFilePathCpp.empty() ? formFilePathCpp : objectFilePathCpp;
        std::wstring includeFileName = GetFileName(filePathCpp);
        Replace(includeFileName, L".cpp", L".hpp");

        LogService::LogInfo(logConfig, LOG_ID, L"Generate object class file: " + filePathCpp);
        
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
        //                               Action  Files                                                //
        // ------------------------------------------------------------------------------------------ //
        std::vector<std::wstring> actionClassList;
        for (auto const &enumClass : enumClassList)
            VPGActionFileGenerationService::GenerateCpp(logConfig, classPathMapping, enumClass.get(), classPrefix, actionFolderPathCpp, actionClassList, systemIncludeFiles, customIncludeFiles);      

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

        content += GetCppCustomHeader(isIncludeForm)
            + L"\r\n"
            "using namespace vcc;\r\n";

        for (auto const &str : actionClassList)
            content += L"\r\n"
                + str;
            
        // Generate Part
        for (auto const &enumClass : enumClassList) {
            if (!IsEndWith(enumClass->GetName(), propertyClassNameSuffix))
                continue;
                
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - propertyClassNameSuffix.length());

            std::wstring baseClassName = (enumClass->GetType() == VPGEnumClassType::Form ? L"BaseForm" : L"BaseObject");
            if (!IsBlank(enumClass->GetInheritClass()))
                baseClassName = enumClass->GetInheritClass();                
            std::wstring baseClassNameWithoutQuote = baseClassName;
            if (IsContain(baseClassNameWithoutQuote, L"<"))
                baseClassNameWithoutQuote = baseClassNameWithoutQuote.substr(0, Find(baseClassNameWithoutQuote, L"<"));

            content += GetCppConstructor(enumClass.get(), classPrefix, className, baseClassNameWithoutQuote)
                + GetCppCloneFunctions(enumClass.get(), className)
                + GetCppJsonFunction(className, enumClassMapping, enumClass)
                + GetCppInitialize(enumClass.get(), className, baseClassNameWithoutQuote)
                + GetCppAction(enumClass.get(), className);
        }

        content += GetCppCustomFunction(isIncludeForm);
            
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
