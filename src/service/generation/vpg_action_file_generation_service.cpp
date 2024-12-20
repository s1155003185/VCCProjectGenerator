#include "vpg_action_file_generation_service.hpp"

#include <assert.h>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#include "vpg_class_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_file_generation_service.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_tag_helper.hpp"

using namespace vcc;

#define LOG_ID L"Action File Generation"

std::wstring VPGActionFileGenerationService::GetActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix)
{
    std::wstring result = L"";
    TRY
        std::wstring currentActionClassName = actionClassName;
        std::wstring projectPrefixLower = projectPrefix;
        ToLower(projectPrefixLower);
        if (!IsBlank(projectPrefix) && IsStartWith(actionClassName, projectPrefix)) {
            result += projectPrefixLower;
            currentActionClassName = currentActionClassName.substr(projectPrefix.length());
        }
        auto tokens = SplitStringByUpperCase(currentActionClassName, false, true);
        for (auto token : tokens) {
            ToLower(token);
            if (!result.empty())
                result += L"_";
            result += token;
        }
    CATCH
    return result;
}

std::vector<std::wstring> VPGActionFileGenerationService::GenerateHpp(const LogConfig *logConfig, const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathHpp)
{
    std::vector<std::wstring> contents;
    TRY
        assert(enumClass != nullptr);        
        std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName(), projectPrefix);
        if (!IsPropertyClass(className, projectPrefix))
            return contents;

        bool isSeperateFile = !IsBlank(folderPathHpp);
        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassPropertyType::Action)
                continue;
            
            std::wstring actionClassName = className + property->GetPropertyName();

            std::wstring propertyStr = L"";
            std::wstring assignmentStr = L"";

            std::wstring action = L"class " + actionClassName + L" : public BaseAction\n"
                "{\r\n"
                + propertyStr
                + INDENT + GetVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, actionClassName)
                + INDENT + GetVccTagTailerCustomClassProperties(VPGCodeType::Cpp, actionClassName)
                + INDENT + L"private:\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName)
                + INDENT + INDENT + GetVccTagTailerCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName)
                + INDENT + INDENT
                + INDENT + L"protected:\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetRedoMessageStart() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetRedoMessageComplete() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetUndoMessageStart() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetUndoMessageComplete() const override;\r\n"
                "\r\n"
                + INDENT + INDENT + L"virtual void OnRedo() const override;\r\n"
                + INDENT + INDENT + L"virtual void OnUndo() const override;\r\n"
                "\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName)
                + INDENT + INDENT + GetVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName)
                + L"\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + actionClassName + L"() : BaseAction() {}\r\n"
                + INDENT + INDENT + actionClassName + L"(LogConfig logConfig, std::shared_ptr<BaseForm> parentForm" + assignmentStr + L");"
                + L"\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName)
                + INDENT + INDENT + GetVccTagTailerCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName)
                + L"}\r\n";

            if (isSeperateFile) {
                // Generate to seperate files
                std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
                    "#pragma once\r\n";


                // Generate File
                std::wstring filePathHpp = ConcatPaths({folderPathHpp, GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp"});
                LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathHpp);
                if (IsFileExists(filePathHpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, ReadFile(filePathHpp), VPGFileContentSyncMode::Full, L"//");
                LTrim(content);
                WriteFile(filePathHpp, content, true);
                LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                contents.push_back(action);
            }

        }
    CATCH
    return contents;
}

std::vector<std::wstring> VPGActionFileGenerationService::GenerateCpp(const LogConfig *logConfig, const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathCpp)
{
    std::vector<std::wstring> contents;
    TRY
        assert(enumClass != nullptr);
    CATCH
    return contents;
}