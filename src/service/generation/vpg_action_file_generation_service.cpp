#include "vpg_action_file_generation_service.hpp"

#include <assert.h>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"
#include "string_helper.hpp"

#include "vpg_class_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_file_generation_service.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_object_file_generation_service.hpp"
#include "vpg_tag_helper.hpp"

using namespace vcc;

#define LOG_ID L"Action File Generation"

void VPGActionFileGenerationService::GenerateHpp(const LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathHpp,
    std::vector<std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->GetName(), projectPrefix))
            return;

        std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName(), projectPrefix);
        bool isSeperateFile = !IsBlank(folderPathHpp);

        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassPropertyType::Action)
                continue;
            
            // include files
            std::set<std::wstring> systemIncludeFiles;
            systemIncludeFiles.insert(L"memory");
            systemIncludeFiles.insert(L"string");

            std::set<std::wstring> customIncludeFiles;
            customIncludeFiles.insert(L"base_action.hpp");
            customIncludeFiles.insert(L"i_object.hpp");
            customIncludeFiles.insert(L"log_config.hpp");

            // class name
            std::wstring actionClassName = className + property->GetPropertyName();

            std::wstring propertyStr = L"";
            std::wstring assignmentStr = L"std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm";
            for (auto const &property : property->GetClassProperties())
            {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Manager && property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;

                propertyStr += INDENT + property->GetMacro() + L"\r\n";

                std::wstring type1 = property->GetType1();
                std::wstring type2 = property->GetType2();
                if (!type1.empty() && IsCaptial(type1))
                    customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type1));
                if (!type2.empty() && IsCaptial(type2))
                    customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type2));

                if (!assignmentStr.empty())
                    assignmentStr += L", ";

                if (property->GetPropertyType() == VPGEnumClassPropertyType::Manager) {
                    assignmentStr += L"std::shared_ptr<" + property->GetType1() + L">";
                } else if (property->GetPropertyType() == VPGEnumClassPropertyType::Property) {
                    if (IsStartWith(property->GetMacro(), L"MAP")) {
                        systemIncludeFiles.insert(L"map");
                        assignmentStr += L"std::map<" + type1 + L", " + (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type2 + L">") : type2) + L">";
                    } else if (IsStartWith(property->GetMacro(), L"VECTOR")) {
                        systemIncludeFiles.insert(L"vector");
                        assignmentStr += L"std::vector<" + (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type1 + L">") : type1) + L">";
                    } else if (IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                        systemIncludeFiles.insert(L"map");
                        systemIncludeFiles.insert(L"vector");
                        assignmentStr += L"std::vector<std::pair<" + type1 + L", "+ (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type2 + L">") : type2) + L">>";
                    } else {
                        assignmentStr += IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type1 + L">") : type1;
                    }
                }
                std::wstring propertyName = property->GetPropertyName();
                ToCamel(propertyName);
                assignmentStr += L" " + propertyName;
            }

            std::wstring action = L"class " + actionClassName + L" : public BaseAction\r\n"
                "{\r\n"
                + propertyStr
                + (!propertyStr.empty() ? L"\r\n" : L"")
                + INDENT + GetVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + GetVccTagTailerCustomClassProperties(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                L"\r\n"
                + INDENT + L"private:\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                "\r\n"
                + INDENT + L"protected:\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetRedoMessageStart() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetRedoMessageComplete() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetUndoMessageStart() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring GetUndoMessageComplete() const override;\r\n"
                "\r\n"
                + INDENT + INDENT + L"virtual void OnRedo() const override;\r\n"
                + INDENT + INDENT + L"virtual void OnUndo() const override;\r\n"
                "\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                "\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + actionClassName + L"() : BaseAction() {}\r\n"
                + INDENT + INDENT + actionClassName + L"(" + assignmentStr + L");\r\n"
                + INDENT + INDENT + L"~" + actionClassName + L"() {}\r\n"
                "\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + L"};\r\n";

            if (isSeperateFile) {
                // Generate to seperate files
                std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
                    "#pragma once\r\n"
                    "\r\n";
                // system include files
                for (auto const &str : systemIncludeFiles)
                    content += L"#include <" + str + L">\r\n";

                // custom include files
                content += L"\r\n";
                for (auto const &str : customIncludeFiles)
                    content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L"\r\n";

                content += L"\r\n"
                    + GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    "\r\n"
                    + action;
                    
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
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());
                actionClassList.push_back(action);
            }

        }
    CATCH
}

void VPGActionFileGenerationService::GenerateCpp(const LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathCpp,
    std::vector<std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->GetName(), projectPrefix))
            return;

        std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName(), projectPrefix);
        bool isSeperateFile = !IsBlank(folderPathCpp);

        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassPropertyType::Action)
                continue;
            
            // include files
            std::set<std::wstring> systemIncludeFiles;
            systemIncludeFiles.insert(L"memory");
            systemIncludeFiles.insert(L"string");

            std::set<std::wstring> customIncludeFiles;
            customIncludeFiles.insert(L"base_action.hpp");
            customIncludeFiles.insert(L"exception_macro.hpp");
            customIncludeFiles.insert(L"i_object.hpp");
            customIncludeFiles.insert(L"log_config.hpp");

            // class name
            std::wstring actionClassName = className + property->GetPropertyName();

            std::wstring assignmentStr = L"std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm";
            std::vector<std::wstring> propertyAssignments;
            propertyAssignments.push_back(L"_LogConfig = logConfig");
            propertyAssignments.push_back(L"_ParentObject = parentForm");
            for (auto const &property : property->GetClassProperties())
            {
                if (property->GetPropertyType() != VPGEnumClassPropertyType::Manager && property->GetPropertyType() != VPGEnumClassPropertyType::Property)
                    continue;

                std::wstring type1 = property->GetType1();
                std::wstring type2 = property->GetType2();
                if (!type1.empty() && IsCaptial(type1))
                    customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type1));
                if (!type2.empty() && IsCaptial(type2))
                    customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type2));

                if (!assignmentStr.empty())
                    assignmentStr += L", ";

                if (property->GetPropertyType() == VPGEnumClassPropertyType::Manager) {
                    assignmentStr += L"std::shared_ptr<" + property->GetType1() + L">";
                } else if (property->GetPropertyType() == VPGEnumClassPropertyType::Property) {
                    if (IsStartWith(property->GetMacro(), L"MAP")) {
                        systemIncludeFiles.insert(L"map");
                        assignmentStr += L"std::map<" + type1 + L", " + (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type2 + L">") : type2) + L">";
                    } else if (IsStartWith(property->GetMacro(), L"VECTOR")) {
                        systemIncludeFiles.insert(L"vector");
                        assignmentStr += L"std::vector<" + (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type1 + L">") : type1) + L">";
                    } else if (IsStartWith(property->GetMacro(), L"ORDERED_MAP")) {
                        systemIncludeFiles.insert(L"map");
                        systemIncludeFiles.insert(L"vector");
                        assignmentStr += L"std::vector<std::pair<" + type1 + L", "+ (IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type2 + L">") : type2) + L">>";
                    } else {
                        assignmentStr += IsContain(property->GetMacro(), L"SPTR") ? (L"std::shared_ptr<" + type1 + L">") : type1;
                    }
                }
                std::wstring propertyName = property->GetPropertyName();
                ToCamel(propertyName);
                assignmentStr += L" " + propertyName;
                propertyAssignments.push_back(L"_" + property->GetPropertyName() + L" = " + propertyName);
            }

            std::wstring action = actionClassName + L"::" + actionClassName + L"(" + assignmentStr + L") : BaseAction()\r\n"
                "{\r\n";
            for (auto const &str : propertyAssignments)
                action += INDENT + str + L";\r\n";
            action += L"}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::GetRedoMessageStart() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::GetRedoMessageComplete() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::GetUndoMessageStart() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::GetUndoMessageComplete() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "void " + actionClassName + L"::OnRedo() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n"
                "\r\n"
                "void " + actionClassName + L"::OnUndo() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                "}\r\n";

            if (isSeperateFile) {
                // Generate to seperate files
                std::wstring fileNameHpp = GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp";
                std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
                    "#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, fileNameHpp) + L"\r\n"
                    "\r\n";
                // system include files
                for (auto const &str : systemIncludeFiles)
                    content += L"#include <" + str + L">\r\n";

                // custom include files
                content += L"\r\n";
                for (auto const &str : customIncludeFiles) {
                    if (str == fileNameHpp)
                        continue;
                    content += L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, str) + L"\r\n";
                }

                content += L"\r\n"
                    + GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    "\r\n"
                    + action
                    + L"\r\n"
                    + GetVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
                    + GetVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";
                    
                // Generate File
                std::wstring filePathCpp = ConcatPaths({folderPathCpp, GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".cpp"});
                LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathCpp);
                if (IsFileExists(filePathCpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, ReadFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
                LTrim(content);
                WriteFile(filePathCpp, content, true);
                LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());
                actionClassList.push_back(action);
            }
        }
    CATCH
}