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
#include "vpg_cpp_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_file_generation_service.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_object_file_generation_service.hpp"
#include "vpg_tag_helper.hpp"

#define LOG_ID L"Action File Generation"

void VPGActionFileGenerationService::GenerateHpp(const vcc::LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathHpp,
    std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->GetName(), projectPrefix))
            return;

        std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName());
        bool isSeperateFile = !vcc::IsBlank(folderPathHpp);

        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassAttributeType::Action)
                continue;
            
            // include files
            std::set<std::wstring> systemIncludeFiles;
            systemIncludeFiles.insert(L"memory");
            systemIncludeFiles.insert(L"string");

            std::set<std::wstring> customIncludeFiles;
            customIncludeFiles.insert(L"base_action.hpp");
            customIncludeFiles.insert(L"i_object.hpp");
            customIncludeFiles.insert(L"i_result.hpp");
            customIncludeFiles.insert(L"log_config.hpp");

            // class name
            std::wstring currentNamespace = GetNamespaceFromClassName(enumClass->GetName());
            std::wstring actionClassName = GetTypeOrClassWithoutNamespace(className) + property->GetPropertyName();

            std::wstring propertyStr = L"";
            std::wstring assignmentStrSimple = L"std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm";
            std::wstring assignmentStr = assignmentStrSimple;
            
            std::wstring type1 = property->GetType1();
            // Only Support shared point argument
            if (!type1.empty() && vcc::IsCapital(type1)) {
                customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type1));
                
                propertyStr = INDENT + L"GETSET_SPTR_NULL(" + type1 + L", Argument)\r\n";

                if (!assignmentStr.empty())
                    assignmentStr += L", ";
                assignmentStr += L"std::shared_ptr<" + type1 + L"> argument";
            }

            std::wstring action = L"\r\n"
                "class " + actionClassName + L" : public vcc::BaseAction\r\n"
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
                + INDENT + INDENT + L"virtual std::wstring GetRedoMessageComplete() const override;\r\n";
            
            if (!property->GetIsNoHistory())
                action += INDENT + INDENT + L"virtual std::wstring GetUndoMessageStart() const override;\r\n"
                    + INDENT + INDENT + L"virtual std::wstring GetUndoMessageComplete() const override;\r\n";
                
            action += L"\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<vcc::IResult> OnRedo() override;\r\n";

            if (!property->GetIsNoHistory())
                action += INDENT + INDENT + L"virtual std::shared_ptr<vcc::IResult> OnUndo() override;\r\n";
            
            action += L"\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                "\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + actionClassName + L"() : vcc::BaseAction() {}\r\n"
                + INDENT + INDENT + actionClassName + L"(" + assignmentStrSimple + L");\r\n";
                if (assignmentStr != assignmentStrSimple)
                    action += INDENT + INDENT + actionClassName + L"(" + assignmentStr + L");\r\n";
                action += INDENT + INDENT + L"~" + actionClassName + L"() {}\r\n"
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
                    content += L"#include " + vcc::GetEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";

                content += L"\r\n"
                    + GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";

                std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
                namespaceClassMapping.insert({ currentNamespace, { action } });
                content += GenerateCodeWithNamespace(namespaceClassMapping);
                    
                // Generate File
                std::wstring filePathHpp = vcc::ConcatPaths({folderPathHpp, GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp"});
                vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathHpp);
                if (vcc::IsFilePresent(filePathHpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::ReadFile(filePathHpp), VPGFileContentSyncMode::Full, L"//");
                vcc::LTrim(content);
                vcc::WriteFile(filePathHpp, content, true);
                vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());
                actionClassList.insert({(!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + actionClassName, action});
            }

        }
    CATCH
}

void VPGActionFileGenerationService::GenerateCpp(const vcc::LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathCpp,
    std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->GetName(), projectPrefix))
            return;

        std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName());
        bool isSeperateFile = !vcc::IsBlank(folderPathCpp);

        for (auto const &property : enumClass->GetProperties()) {
            if (property->GetPropertyType() != VPGEnumClassAttributeType::Action)
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
            customIncludeFiles.insert(L"i_result.hpp");

            // class name
            std::wstring currentNamespace = GetNamespaceFromClassName(enumClass->GetName());
            std::wstring actionClassName = GetTypeOrClassWithoutNamespace(className) + property->GetPropertyName();

            std::wstring assignmentStrSimple = L"std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm";
            std::wstring assignmentStr = assignmentStrSimple;
            std::vector<std::wstring> propertyAssignmentsSimple;
            propertyAssignmentsSimple.push_back(L"_LogConfig = logConfig");
            propertyAssignmentsSimple.push_back(L"_ParentObject = parentForm");
            std::vector<std::wstring> propertyAssignments;
            propertyAssignments.insert(propertyAssignments.end(), propertyAssignmentsSimple.begin(), propertyAssignmentsSimple.end());

            std::wstring type1 = property->GetType1();
            if (!type1.empty() && vcc::IsCapital(type1)) {
                customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, type1));
                
                if (!assignmentStr.empty())
                    assignmentStr += L", ";

                assignmentStr += L"std::shared_ptr<" + property->GetType1() + L"> argument";
                propertyAssignments.push_back(L"_Argument = argument");
            }
            if (!vcc::IsBlank(property->GetActionResultRedoClass()))
                customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, property->GetActionResultRedoClass()));
            if (!vcc::IsBlank(property->GetActionResultUndoClass()))
                customIncludeFiles.insert(VPGObjectFileGenerationService::GetProjectClassIncludeFile(classPathMapping, property->GetActionResultUndoClass()));


            std::wstring action = L"\r\n"
                + actionClassName + L"::" + actionClassName + L"(" + assignmentStrSimple + L") : vcc::BaseAction()\r\n"
            "{\r\n";
            for (auto const &str : propertyAssignmentsSimple)
                action += INDENT + str + L";\r\n";
            action += L"}\r\n";
            if (assignmentStrSimple != assignmentStr) {
                action += L"\r\n" + actionClassName + L"::" + actionClassName + L"(" + assignmentStr + L") : vcc::BaseAction()\r\n"
                "{\r\n";
                for (auto const &str : propertyAssignments)
                    action += INDENT + str + L";\r\n";
                action += L"}\r\n";
            }

            action += L"\r\n"
                "std::wstring " + actionClassName + L"::GetRedoMessageStart() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + INDENT + L"return L\"Execute " + actionClassName + L" start\";\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::GetRedoMessageComplete() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + INDENT + L"return L\"Execute " + actionClassName + L" complete\";\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n";
            
            if (!property->GetIsNoHistory())
                action += L"\r\n"
                    "std::wstring " + actionClassName + L"::GetUndoMessageStart() const\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                    + INDENT + INDENT + L"return L\"Undo " + actionClassName + L" start\";\r\n"
                    + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                    + INDENT + L"CATCH\r\n"
                    + INDENT + L"return L\"\";\r\n"
                    "}\r\n"
                    "\r\n"
                    "std::wstring " + actionClassName + L"::GetUndoMessageComplete() const\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                    + INDENT + INDENT + L"return L\"Undo " + actionClassName + L" complete\";\r\n"
                    + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                    + INDENT + L"CATCH\r\n"
                    + INDENT + L"return L\"\";\r\n"
                    "}\r\n";

            std::wstring redoReturnClass = !vcc::IsBlank(property->GetActionResultRedoClass()) ? property->GetActionResultRedoClass() : L"vcc::OperationResult";
            action += L"\r\n"
                "std::shared_ptr<vcc::IResult> " + actionClassName + L"::OnRedo()\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + L"CATCH_RETURN_RESULT(" + redoReturnClass + L")\r\n"
                + INDENT + L"return std::make_shared<" + redoReturnClass + L">();\r\n"
                "}\r\n";

            if (!property->GetIsNoHistory()) {
                std::wstring undoReturnClass = !vcc::IsBlank(property->GetActionResultUndoClass()) ? property->GetActionResultUndoClass() : L"vcc::OperationResult";
                action += L"\r\n"
                    "std::shared_ptr<vcc::IResult> " + actionClassName + L"::OnUndo()\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + GetVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                    + INDENT + INDENT + GetVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                    + INDENT + L"CATCH_RETURN_RESULT(" + undoReturnClass + L")\r\n"
                    + INDENT + L"return std::make_shared<" + undoReturnClass + L">();\r\n"
                    "}\r\n";
            }

            if (isSeperateFile) {
                // Generate to seperate files
                std::wstring fileNameHpp = GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp";
                std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
                    "#include " + vcc::GetEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, fileNameHpp) + L"\r\n"
                    "\r\n";
                // system include files
                for (auto const &str : systemIncludeFiles)
                    content += L"#include <" + str + L">\r\n";

                // custom include files
                content += L"\r\n";
                for (auto const &str : customIncludeFiles) {
                    if (str == fileNameHpp)
                        continue;
                    content += L"#include " + vcc::GetEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";
                }

                std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
                namespaceClassMapping.insert({ currentNamespace, { action } });
                content += L"\r\n"
                    + GetVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + GetVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + GenerateCodeWithNamespace(namespaceClassMapping)
                    + L"\r\n"
                    + GetVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
                    + GetVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";
                    
                // Generate File
                std::wstring filePathCpp = vcc::ConcatPaths({folderPathCpp, GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".cpp"});
                vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathCpp);
                if (vcc::IsFilePresent(filePathCpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::ReadFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
                vcc::LTrim(content);
                vcc::WriteFile(filePathCpp, content, true);
                vcc::LogService::LogInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());

                actionClassList.insert({(!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + actionClassName, action});
            }
        }
    CATCH
}