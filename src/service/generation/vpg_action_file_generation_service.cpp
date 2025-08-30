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

void VPGActionFileGenerationService::generateHpp(const vcc::LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathHpp,
    std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->getName(), projectPrefix))
            return;

        std::wstring className = getClassNameFromPropertyClassName(enumClass->getName());
        bool isSeperateFile = !vcc::isBlank(folderPathHpp);

        for (auto const &property : enumClass->getProperties()) {
            if (property->getPropertyType() != VPGEnumClassAttributeType::Action)
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
            std::wstring currentNamespace = getNamespaceFromClassName(enumClass->getName());
            std::wstring actionClassName = getTypeOrClassWithoutNamespace(className) + property->getPropertyName();

            std::wstring propertyStr = L"";
            std::wstring assignmentStrSimple = L"std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm";
            std::wstring assignmentStr = assignmentStrSimple;
            
            std::wstring type1 = property->getType1();
            // Only Support shared point argument
            if (!type1.empty() && vcc::isCapital(type1)) {
                customIncludeFiles.insert(VPGObjectFileGenerationService::getProjectClassIncludeFile(classPathMapping, type1));
                
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
                + INDENT + getVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + getVccTagTailerCustomClassProperties(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                L"\r\n"
                + INDENT + L"private:\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassPrivateFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                "\r\n"
                + INDENT + L"protected:\r\n"
                + INDENT + INDENT + L"virtual std::wstring getRedoMessageStart() const override;\r\n"
                + INDENT + INDENT + L"virtual std::wstring getRedoMessageComplete() const override;\r\n";
            
            if (!property->getIsNoHistory())
                action += INDENT + INDENT + L"virtual std::wstring getUndoMessageStart() const override;\r\n"
                    + INDENT + INDENT + L"virtual std::wstring getUndoMessageComplete() const override;\r\n";
                
            action += L"\r\n"
                + INDENT + INDENT + L"virtual std::shared_ptr<vcc::IResult> onRedo() override;\r\n";

            if (!property->getIsNoHistory())
                action += INDENT + INDENT + L"virtual std::shared_ptr<vcc::IResult> onUndo() override;\r\n";
            
            action += L"\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassProtectedFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                "\r\n"
                + INDENT + L"public:\r\n"
                + INDENT + INDENT + actionClassName + L"() : vcc::BaseAction() {}\r\n"
                + INDENT + INDENT + actionClassName + L"(" + assignmentStrSimple + L");\r\n";
                if (assignmentStr != assignmentStrSimple)
                    action += INDENT + INDENT + actionClassName + L"(" + assignmentStr + L");\r\n";
                action += INDENT + INDENT + L"~" + actionClassName + L"() {}\r\n"
                "\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassPublicFunctions(VPGCodeType::Cpp, actionClassName) + L"\r\n"
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
                    content += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";

                content += L"\r\n"
                    + getVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + getVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";

                std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
                namespaceClassMapping.insert({ currentNamespace, { action } });
                content += generateCodeWithNamespace(namespaceClassMapping);
                    
                // Generate File
                std::wstring filePathHpp = vcc::concatPaths({folderPathHpp, getActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp"});
                vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathHpp);
                if (vcc::isFilePresent(filePathHpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathHpp), VPGFileContentSyncMode::Full, L"//");
                vcc::lTrim(content);
                vcc::writeFile(filePathHpp, content, true);
                vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());
                actionClassList.insert({(!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + actionClassName, action});
            }

        }
    CATCH
}

void VPGActionFileGenerationService::generateCpp(const vcc::LogConfig *logConfig,
    const std::map<std::wstring, std::wstring> &classPathMapping,
    const VPGEnumClass *enumClass,
    const std::wstring &projectPrefix, const std::wstring &folderPathCpp,
    std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles)
{
    TRY
        assert(enumClass != nullptr);
        if (!IsPropertyClassNameValidToGenerateClass(enumClass->getName(), projectPrefix))
            return;

        std::wstring className = getClassNameFromPropertyClassName(enumClass->getName());
        bool isSeperateFile = !vcc::isBlank(folderPathCpp);

        for (auto const &property : enumClass->getProperties()) {
            if (property->getPropertyType() != VPGEnumClassAttributeType::Action)
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
            std::wstring currentNamespace = getNamespaceFromClassName(enumClass->getName());
            std::wstring actionClassName = getTypeOrClassWithoutNamespace(className) + property->getPropertyName();

            std::wstring assignmentStrSimple = L"std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm";
            std::wstring assignmentStr = assignmentStrSimple;
            std::vector<std::wstring> propertyAssignmentsSimple;
            propertyAssignmentsSimple.push_back(L"_LogConfig = logConfig");
            propertyAssignmentsSimple.push_back(L"_ParentObject = parentForm");
            std::vector<std::wstring> propertyAssignments;
            propertyAssignments.insert(propertyAssignments.end(), propertyAssignmentsSimple.begin(), propertyAssignmentsSimple.end());

            std::wstring type1 = property->getType1();
            if (!type1.empty() && vcc::isCapital(type1)) {
                customIncludeFiles.insert(VPGObjectFileGenerationService::getProjectClassIncludeFile(classPathMapping, type1));
                
                if (!assignmentStr.empty())
                    assignmentStr += L", ";

                assignmentStr += L"std::shared_ptr<" + property->getType1() + L"> argument";
                propertyAssignments.push_back(L"_Argument = argument");
            }
            if (!vcc::isBlank(property->getActionResultRedoClass()))
                customIncludeFiles.insert(VPGObjectFileGenerationService::getProjectClassIncludeFile(classPathMapping, property->getActionResultRedoClass()));
            if (!vcc::isBlank(property->getActionResultUndoClass()))
                customIncludeFiles.insert(VPGObjectFileGenerationService::getProjectClassIncludeFile(classPathMapping, property->getActionResultUndoClass()));


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
                "std::wstring " + actionClassName + L"::getRedoMessageStart() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + INDENT + L"return L\"execute " + actionClassName + L" start\";\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageStart") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n"
                "\r\n"
                "std::wstring " + actionClassName + L"::getRedoMessageComplete() const\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + INDENT + L"return L\"execute " + actionClassName + L" complete\";\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetRedoMessageComplete") + L"\r\n"
                + INDENT + L"CATCH\r\n"
                + INDENT + L"return L\"\";\r\n"
                "}\r\n";
            
            if (!property->getIsNoHistory())
                action += L"\r\n"
                    "std::wstring " + actionClassName + L"::GetUndoMessageStart() const\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                    + INDENT + INDENT + L"return L\"Undo " + actionClassName + L" start\";\r\n"
                    + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageStart") + L"\r\n"
                    + INDENT + L"CATCH\r\n"
                    + INDENT + L"return L\"\";\r\n"
                    "}\r\n"
                    "\r\n"
                    "std::wstring " + actionClassName + L"::GetUndoMessageComplete() const\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                    + INDENT + INDENT + L"return L\"Undo " + actionClassName + L" complete\";\r\n"
                    + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"GetUndoMessageComplete") + L"\r\n"
                    + INDENT + L"CATCH\r\n"
                    + INDENT + L"return L\"\";\r\n"
                    "}\r\n";

            std::wstring redoReturnClass = !vcc::isBlank(property->getActionResultRedoClass()) ? property->getActionResultRedoClass() : L"vcc::OperationResult";
            action += L"\r\n"
                "std::shared_ptr<vcc::IResult> " + actionClassName + L"::onRedo()\r\n"
                "{\r\n"
                + INDENT + L"TRY\r\n"
                + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnRedo") + L"\r\n"
                + INDENT + L"CATCH_RETURN_RESULT(" + redoReturnClass + L")\r\n"
                + INDENT + L"return std::make_shared<" + redoReturnClass + L">();\r\n"
                "}\r\n";

            if (!property->getIsNoHistory()) {
                std::wstring undoReturnClass = !vcc::isBlank(property->getActionResultUndoClass()) ? property->getActionResultUndoClass() : L"vcc::OperationResult";
                action += L"\r\n"
                    "std::shared_ptr<vcc::IResult> " + actionClassName + L"::onUndo()\r\n"
                    "{\r\n"
                    + INDENT + L"TRY\r\n"
                    + INDENT + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                    + INDENT + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"", actionClassName, L"OnUndo") + L"\r\n"
                    + INDENT + L"CATCH_RETURN_RESULT(" + undoReturnClass + L")\r\n"
                    + INDENT + L"return std::make_shared<" + undoReturnClass + L">();\r\n"
                    "}\r\n";
            }

            if (isSeperateFile) {
                // Generate to seperate files
                std::wstring fileNameHpp = getActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp";
                std::wstring content = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
                    "#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, fileNameHpp) + L"\r\n"
                    "\r\n";
                // system include files
                for (auto const &str : systemIncludeFiles)
                    content += L"#include <" + str + L">\r\n";

                // custom include files
                content += L"\r\n";
                for (auto const &str : customIncludeFiles) {
                    if (str == fileNameHpp)
                        continue;
                    content += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, str) + L"\r\n";
                }

                std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
                namespaceClassMapping.insert({ currentNamespace, { action } });
                content += L"\r\n"
                    + getVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + getVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n"
                    + generateCodeWithNamespace(namespaceClassMapping)
                    + L"\r\n"
                    + getVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
                    + getVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";
                    
                // Generate File
                std::wstring filePathCpp = vcc::concatPaths({folderPathCpp, getActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".cpp"});
                vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate action class file: " + filePathCpp);
                if (vcc::isFilePresent(filePathCpp))
                    content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
                vcc::lTrim(content);
                vcc::writeFile(filePathCpp, content, true);
                vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate action class file completed.");
            } else {
                // Generate to form files
                globalSystemIncludeFiles.insert(systemIncludeFiles.begin(), systemIncludeFiles.end());
                globalCustomIncludeFiles.insert(customIncludeFiles.begin(), customIncludeFiles.end());

                actionClassList.insert({(!currentNamespace.empty() ? (currentNamespace + L"::") : L"") + actionClassName, action});
            }
        }
    CATCH
}