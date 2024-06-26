#include "vpg_object_file_generation_service.hpp"

#include <map>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#include "vpg_include_path_service.hpp"

using namespace vcc;

#define LOG_ID L"Object File Generation"
const std::wstring proeprtyClassNameSuffix = L"Property";

bool VPGObjectFileGenerationService::IsJsonObject(const VPGEnumClass* enumClass)
{
    TRY
        return IsContainSubstring(enumClass->GetCommand(), L"@@Json", 0, true);
    CATCH
    return false;
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

void VPGObjectFileGenerationService::GenerateHpp(const LogProperty *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object class file: " + filePathHpp);

        // TODO: need to enable to check all systemn function
        // need to skip all code in command
        // Mac: all next tokens after class _LIBCPP_TEMPLATE_VIS under path /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1
        // Window: C:\msys64\mingw64\include\c++\12.2.0

        std::set<std::wstring> systemFileList;
        std::set<std::wstring> projectFileList;
        std::set<std::wstring> abstractClassList;

        projectFileList.insert(L"#include \"base_object.hpp\"");
        projectFileList.insert(L"#include \"class_macro.hpp\"");
        projectFileList.insert(L"#include \"object_type.hpp\"");
        // generate external class
        for (auto const &enumClass : enumClassList) {
            // Json
            bool isJsonObject = VPGObjectFileGenerationService::IsJsonObject(enumClass.get());
            if (isJsonObject) {
                projectFileList.insert(L"#include \"base_json_object.hpp\"");
                projectFileList.insert(L"#include \"json.hpp\"");
                projectFileList.insert(L"#include \"i_document.hpp\"");
            }
            
            for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                std::wstring type = property->GetType1();
                if (std::iswupper(type[0])) {
                    if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {

                        std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
                        else
                            abstractClassList.insert(L"class " + type + L";");
                    } else {
                        std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
                        else
                            abstractClassList.insert(L"enum class " + type + L";");
                    }
                } else {
                    // TODO: need to enable to check all systemn function
                    // system type
                    if (CountSubstring(type, L"string") > 0)
                        systemFileList.insert(L"#include <string>");
                }

                type = property->GetType2();
                if (!type.empty() && std::iswupper(type[0])) {
                    if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {

                        std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
                        else
                            abstractClassList.insert(L"class " + type + L";");
                    } else {
                        std::wstring includeFile = VPGObjectFileGenerationService::GetProjectClassIncludeFile(projectClassIncludeFiles, type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
                        else
                            abstractClassList.insert(L"enum class " + type + L";");
                    }
                } else {
                    // only suuport string
                    // system type
                    if (CountSubstring(type, L"string") > 0)
                        systemFileList.insert(L"#include <string>");
                }
            }
        }
        std::wstring systemFileListStr = L"";
        std::wstring projectFileListStr = L"";
        for (auto const &str : systemFileList)
            systemFileListStr += str + L"\r\n";
        for (auto const &str : projectFileList)
            projectFileListStr += str + L"\r\n";

        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += !systemFileListStr.empty() ? (systemFileListStr + L"\r\n") : L"";
        content += !projectFileListStr.empty() ? (projectFileListStr + L"\r\n") : L"";
        
        // as using base_object and macro, must have namespace vcc
        content += L"using namespace vcc;\r\n";
        // for those class that cannot be found in file list
        for (auto const &str : abstractClassList)
            content +=  L"\r\n" + str;
            
        // generate class
        for (auto const &enumClass : enumClassList) {
            std::wstring inheritClass = L"";
            // Json
            bool isJsonObject = VPGObjectFileGenerationService::IsJsonObject(enumClass.get());
            if (isJsonObject)
                inheritClass += L", public BaseJsonObject";

            content += L"\r\n";
            
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - proeprtyClassNameSuffix.length());
            content += L"class " + className + L" : public BaseObject<" + className + L">" + inheritClass + L"\r\n";
            content += L"{\r\n";
            // generate properties
            for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                content += INDENT + property->GetMacro() + L"\r\n";
            }
            content += L"\r\n";
            content += INDENT + L"public:\r\n";
            content += INDENT + INDENT + className + L"() : BaseObject(ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L") {}\r\n";
            content += INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n";

            bool isPtrExists = false;
            for (auto const &property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                if ((!property->GetType1().empty() && std::iswupper(property->GetType1()[0])) 
                    || (!property->GetType2().empty() && std::iswupper(property->GetType2()[0]))) {
                    if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {
                        isPtrExists = true;
                        break;
                    }
                }
            }
            if (isPtrExists) {
                content += L"\r\n";                
                content += INDENT + INDENT + L"virtual std::shared_ptr<IObject> Clone() const override {\r\n";
                content += INDENT + INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::make_shared<" + className + L">(*this);\r\n";
                for (auto const &property : enumClass->GetProperties()) {
                    // handle enum without macro case
                    if (property->GetMacro().empty())
                        continue;
                    if ((!property->GetType1().empty() && std::iswupper(property->GetType1()[0])) 
                        || (!property->GetType2().empty() && std::iswupper(property->GetType2()[0]))) {
                        if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
                            content += INDENT + INDENT + INDENT + L"obj->Clone" + property->GetPropertyName() + L"(this->_" + property->GetPropertyName() + L");\r\n";
                        }
                    }
                }
                content += INDENT + INDENT + INDENT + L"return obj;\r\n";
                content += INDENT + INDENT + L"}\r\n";
            }
            if (isJsonObject) {
                content += L"\r\n"
                    + INDENT + INDENT + L"virtual std::shared_ptr<Json> ToJson() const override;\r\n"
                    + INDENT + INDENT + L"virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;\r\n";
            }

            content += L"};\r\n";
        }
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object class file completed.");
    CATCH
}

void VPGObjectFileGenerationService::GenerateCpp(const LogProperty *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
    const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        bool isContainJsonObject = false;
        for (auto const &enumClass : enumClassList) {
            isContainJsonObject = VPGObjectFileGenerationService::IsJsonObject(enumClass.get());
            if (isContainJsonObject)
                break;
        }
        if (!isContainJsonObject)
            return;
        
        std::wstring includeFileName = GetFileName(filePathCpp);
        Replace(includeFileName, L".hpp", L".cpp");

        std::set<std::wstring> systemIncludeFiles;
        std::set<std::wstring> customIncludeFiles;

        systemIncludeFiles.insert(L"assert.h");
        systemIncludeFiles.insert(L"memory");
        systemIncludeFiles.insert(L"string");
        
        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_document.hpp");
        customIncludeFiles.insert(L"i_document_builder.hpp");
        customIncludeFiles.insert(L"json.hpp");
        customIncludeFiles.insert(L"memory_macro.hpp");
        customIncludeFiles.insert(L"string_helper.hpp");
        
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object class file: " + filePathCpp);
        std::wstring content = L"#include \"" + includeFileName + L"\"\r\n";

        if (!systemIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &str : systemIncludeFiles)
                content += L"#include <" + str + L">\r\n";
        }
        if (!customIncludeFiles.empty()) {
            content += L"\r\n";
            for (auto const &str : customIncludeFiles)
                content += L"#include \"" + str + L"\"\r\n";
        }
        content += L"\r\nusing namespace vcc;\r\n";

        for (auto const &enumClass : enumClassList) {
            if (!VPGObjectFileGenerationService::IsJsonObject(enumClass.get()))
                continue;
            content += L"\r\n"
                "std::shared_ptr<Json> " + enumClass->GetName() + L"::ToJson() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"DECLARE_UPTR(Json, json);\r\n"
                // TODO
                + INDENT + INDENT + L"return json;\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return nullptr;\r\n"
                "}\r\n"
                "\r\n"
                "void " + enumClass->GetName() + L"::DeserializeJson(std::shared_ptr<IDocument> document) const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + L"std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);\r\n"
                + INDENT + INDENT + L"assert(json != nullptr);\r\n"
                //  TODO
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return nullptr;\r\n"
                "}\r\n";
            
        }

        WriteFile(filePathCpp, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object class file completed.");
    CATCH
}