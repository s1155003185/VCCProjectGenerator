#include "vpg_base_generation_manager.hpp"

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "base_json_object.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_global.hpp"
#include "vpg_project_type.hpp"

void VPGBaseGenerationManager::ValidateOption() const
{
    TRY
        if (IsBlank(_Option->GetTemplateWorkspace()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Template Workspace is emtpy.");
        if (IsBlank(_Workspace))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace is emtpy.");
        if (IsBlank(_Option->GetProjectName()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Project Name is emtpy.");
        if (IsBlank(_Option->GetProjectNameDll()) && IsBlank(_Option->GetProjectNameExe()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Both DLL name and EXE name both empty.");
    CATCH
}

void VPGBaseGenerationManager::GetDLLTestFileContent(std::wstring &fileContent) const
{
    ReplaceRegex(fileContent, L"#define DLL_NAME L\"([^\"]*)\"", L"#define DLL_NAME L\"" + _Option->GetProjectNameDll() + L"\"");
}

void VPGBaseGenerationManager::CreateWorkspaceDirectory() const
{
    TRY
        ValidateOption();
        // All type has same project structure
        std::vector<std::wstring> checkList;
        // bin
        checkList.push_back(L"bin");
        checkList.push_back(L"bin/Debug");
        checkList.push_back(L"bin/Release");

        // include
        checkList.push_back(L"include");

        // lib
        checkList.push_back(L"lib");

        // src
        checkList.push_back(L"src");

        // unittest
        if (!_Option->GetIsExcludeUnittest())
            checkList.push_back(unittestFolderName);
        
        for (auto path : checkList) {
            std::wstring absPath = ConcatPaths({_Workspace, path});
            if (!IsDirectoryExists(absPath)) {
                CreateDirectory(absPath);
                LogService::LogInfo(this->GetLogConfig().get(), L"", L"Create Directory: " + path);
            }        
        }
    CATCH
}
void VPGBaseGenerationManager::CreateBasicProject() const
{
    TRY
        ValidateOption();
        this->CreateWorkspaceDirectory();

        std::wstring src = VPGGlobal::GetConvertedPath(_Option->GetTemplateWorkspace());
        std::wstring dest = _Workspace;
        if (_Option->GetIsGit()) {
            CopyFile(ConcatPaths({src, L".gitignore"}), ConcatPaths({dest, L".gitignore"}), true);
        }
        if (!IsBlank(_Option->GetProjectNameExe())) {
            CopyFile(ConcatPaths({src, L"main.cpp"}), ConcatPaths({dest, L"main.cpp"}), true);
        }
        if (!IsBlank(_Option->GetProjectNameDll())) {
            CopyFile(ConcatPaths({src, L"DllEntryPoint.cpp"}), ConcatPaths({dest, L"DllEntryPoint.cpp"}), true);
            CopyFile(ConcatPaths({src, L"DllFunctions.cpp"}), ConcatPaths({dest, L"DllFunctions.cpp"}), true);
            CopyFile(ConcatPaths({src, L"DllFunctions.h"}), ConcatPaths({dest, L"DllFunctions.h"}), true);
        }
        if (!_Option->GetIsExcludeUnittest()) {
            CopyFile(ConcatPaths({src, unittestFolderName, L"gtest_main.cpp"}), ConcatPaths({dest, unittestFolderName, L"gtest_main.cpp"}), true);

            if (!IsBlank(_Option->GetProjectNameDll())) {
                std::wstring dllUnitTestContent = ReadFile(ConcatPaths({src, unittestFolderName, L"Dll/dll_test.cpp"}));
                GetDLLTestFileContent(dllUnitTestContent);
                AppendFileOneLine(ConcatPaths({dest, unittestFolderName, L"Dll/dll_test.cpp"}), dllUnitTestContent, true);
            }
        }
        // Cannot Copy
        AppendFileOneLine(ConcatPaths({dest, L"LICENSE"}), L"", true);
        AppendFileOneLine(ConcatPaths({dest, L"README.md"}), L"", true);
        AppendFileOneLine(ConcatPaths({dest, L"CHANGELOG.md"}), L"", true);

        if (IsFilePresent(ConcatPaths({src, MakeFileName}))) {
            // Makefile
            std::wstring makefilePath = ConcatPaths({dest, MakeFileName});
            std::wstring makefileContent = ReadFile(ConcatPaths({src, MakeFileName}));
            WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        }
        
        // .vscode
        std::wstring launchFilePath = ConcatPaths({dest,  L".vscode/launch.json"});
        std::wstring launchFileContent = ReadFile(ConcatPaths({src,  L".vscode/launch.json"}));
        WriteFile(launchFilePath, this->AdjustVSCodeLaunchJson(launchFileContent), true);
        
        std::wstring tasksFilePath = ConcatPaths({dest,  L".vscode/tasks.json"});
        std::wstring tasksFileContent = ReadFile(ConcatPaths({src,  L".vscode/tasks.json"}));
        WriteFile(tasksFilePath, tasksFileContent, true);   
    CATCH
}

void VPGBaseGenerationManager::SyncWorkspace(const LogConfig *logConfig, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters) const
{
    TRY
        std::vector<std::wstring> needToAdd, needToModify, needToDelete;
        TRY
            GetFileDifferenceBetweenWorkspaces(sourceWorkspace, targetWorkspace, needToAdd, needToModify, needToDelete);
        CATCH

        // Delete
        for (auto path : needToDelete) {
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(targetPath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    targetPath += L"/";
                }                
            }

            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;


            if (IsFile(targetPath)) {
                remove(PATH(targetPath));
                LogService::LogInfo(logConfig, L"", L"Removed Directory: " + targetPath);
            } else {
                std::filesystem::remove_all(PATH(targetPath));
                LogService::LogInfo(logConfig, L"", L"Removed File: " + targetPath);
            }
        }

        // Add
        for (auto path : needToAdd) {
            std::wstring sourcePath = ConcatPaths({sourceWorkspace, path});
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!std::filesystem::exists(sourcePath))
                continue;
            if (std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(sourcePath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    sourcePath += L"/";
                    targetPath += L"/";
                }                
            }

            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;

            if (IsFile(sourcePath)) {
                VPGFileSyncService::CopyFile(logConfig, VPGFileContentSyncTagMode::Synchronization, sourcePath, targetPath);
            } else {
                CreateDirectory(targetPath);
                LogService::LogInfo(logConfig, L"", L"Added Directory: " + targetPath);
            }
        }

        // Modify
        for (auto path : needToModify) {
            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;

            std::wstring sourcePath = ConcatPaths({sourceWorkspace, path});
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!IsFile(sourcePath) || !IsFile(targetPath))
                continue;
            
            // modify file
            VPGFileSyncService::CopyFile(logConfig, VPGFileContentSyncTagMode::Synchronization, sourcePath, targetPath);
        }
    CATCH
}

std::wstring VPGBaseGenerationManager::AdjustMakefile(const std::wstring &fileContent) const
{
    ValidateOption();
    std::wstring result = L"";
    TRY
        auto elements = std::make_shared<Xml>();
        VPGCodeReader reader(L"#");
        reader.Deserialize(fileContent, elements);
        for (std::shared_ptr<vcc::Xml> element : elements->GetChildren()) {
            if (element->GetName() == L"vcc:name") {
                std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
                std::wstring dllName = !IsBlank(_Option->GetProjectNameDll()) ? (L" " + _Option->GetProjectNameDll()) : L"";
                std::wstring exeName = !IsBlank(_Option->GetProjectNameExe()) ? (L" " + _Option->GetProjectNameExe()) : L"";
                std::wstring IsExcludeUnittest = _Option->GetIsExcludeUnittest() ? L" Y" : L" N";
        
                result += L"# <vcc:name sync=\"ALERT\" gen=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJ_NAME :=" + projName + L"\r\n";
                result += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
                result += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
                result += L"IS_EXCLUDE_UNITTEST :=" + IsExcludeUnittest + L"\r\n";
                result += L"# </vcc:name>";
            } else if (element->GetName() == L"vcc:export") {
                std::wstring exportDllDirWindow = L"";
                std::wstring exportExeDirWindow = L"";
                std::wstring exportExternalLibDirWindow = L"";
                std::wstring exportDllDirLinux = L"";
                std::wstring exportExeDirLinux = L"";
                std::wstring exportExternalLibDirLinux = L"";

                for (auto const &exportOption : _Option->GetExports()) {
                    if (IsBlank(exportOption->GetWorkspace()))
                        continue;
                    std::wstring workspace = exportOption->GetWorkspace(); //IsAbsolutePath(exportOption->GetWorkspace()) ? exportOption->GetWorkspace() : ConcatPaths({ _Workspace, exportOption->GetWorkspace() });

                    if (!IsBlank(exportOption->GetExportDirectoryDll())) {
                        exportDllDirWindow += L" " + GetWindowPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryDll() }));
                        exportDllDirLinux += L" " + GetLinuxPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryDll() }));
                        if (exportOption->GetIsExportExternalLib()) {
                            exportExternalLibDirWindow += L" " + GetWindowPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryDll() }));
                            exportExternalLibDirLinux += L" " + GetLinuxPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryDll() }));
                        }
                    }
                    if (!IsBlank(exportOption->GetExportDirectoryExe())) {
                        exportExeDirWindow += L" " + GetWindowPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryExe() }));
                        exportExeDirLinux += L" " + GetLinuxPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryExe() }));
                        if (exportOption->GetIsExportExternalLib()) {
                            exportExternalLibDirWindow += L" " + GetWindowPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryExe() }));
                            exportExternalLibDirLinux += L" " + GetLinuxPath(ConcatPaths({ workspace, exportOption->GetExportDirectoryExe() }));                            
                        }
                    }
                }
                ReplaceAll(exportDllDirWindow, L"\\", L"\\\\");
                ReplaceAll(exportExeDirWindow, L"\\", L"\\\\");
                ReplaceAll(exportExternalLibDirWindow, L"\\", L"\\\\");

                result += L"# <vcc:export sync=\"ALERT\" gen=\"ALERT\">\r\n"
                    "#----------------------------------#\r\n"
                    "#---------- Export ----------------#\r\n"
                    "#----------------------------------#\r\n"                
                    "ifeq ($(OS),Windows_NT)\r\n"
                    "EXPORT_DLL_DIR := " + exportDllDirWindow + L"\r\n"
                    "EXPORT_EXE_DIR := " + exportExeDirWindow + L"\r\n"
                    "EXPORT_EXTERNAL_LIB := " + exportExternalLibDirWindow + L"\r\n"
                    "else\r\n"
                    "EXPORT_DLL_DIR := " + exportDllDirLinux + L"\r\n"
                    "EXPORT_EXE_DIR := " + exportExeDirLinux + L"\r\n"
                    "EXPORT_EXTERNAL_LIB := " + exportExternalLibDirLinux + L"\r\n"
                    "endif\r\n"
                    "# </vcc:export>";
            } else
                result += element->GetFullText();
        }
    CATCH
    return result;
}

std::wstring VPGBaseGenerationManager::AdjustVSCodeLaunchJson(const std::wstring &fileContent) const
{
    TRY
        std::wstring programPath = L"${workspaceFolder}/bin/Debug/unittest";
        if (_Option->GetIsExcludeUnittest() && !_Option->GetProjectNameExe().empty()) {
            std::wstring projectName = _Option->GetProjectNameExe();
            #ifdef __WIN32
            projectName += L".exe";
            #endif
            programPath = L"${workspaceFolder}/bin/Debug/" + projectName;
        }

        auto jsonBuilder = std::make_unique<JsonBuilder>();
        jsonBuilder->SetIsBeautify(true);
        jsonBuilder->SetIndent(L"  ");

        auto json = std::make_shared<Json>();
        jsonBuilder->Deserialize(fileContent, json);
        for (std::shared_ptr<Json> element : json->GetArray(L"configurations")) {
            for (std::shared_ptr<Json> arrayElement : element->GetJsonInternalArray()) {
                if (arrayElement->IsContainKey(L"program"))
                    arrayElement->SetString(L"program", programPath);
            }
        }
        return jsonBuilder->Serialize(json.get());
    CATCH
    return fileContent;
}
