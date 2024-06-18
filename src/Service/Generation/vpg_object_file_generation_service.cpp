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

void VPGObjectFileGenerationService::Generate(const LogProperty *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
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
            content += L"\r\n";

            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - proeprtyClassNameSuffix.length());
            content += L"class " + className + L" : public BaseObject<" + className + L">\r\n";
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
            content += L"};\r\n";
        }
        WriteFile(filePathHpp, content, true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate object class file completed.");
    CATCH
}