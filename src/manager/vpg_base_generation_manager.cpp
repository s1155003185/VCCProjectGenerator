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
#include "vpg_config.hpp"
#include "vpg_global.hpp"
#include "vpg_project_type.hpp"

void VPGBaseGenerationManager::validateOption() const
{
    TRY
        VALIDATE(L"Template Workspace is emtpy.", (_Option->getTemplate() != nullptr && !vcc::isBlank(_Option->getTemplate()->getWorkspace())));
        if (vcc::isBlank(_Workspace))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace is emtpy.");
        if (vcc::isBlank(_Option->getProjectName()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Project Name is emtpy.");
        if (vcc::isBlank(_Option->getProjectNameDll()) && vcc::isBlank(_Option->getProjectNameExe()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Both DLL name and EXE name both empty.");
    CATCH
}

void VPGBaseGenerationManager::getDLLTestFileContent(std::wstring &fileContent) const
{
    vcc::replaceRegex(fileContent, L"#define DLL_NAME L\"([^\"]*)\"", L"#define DLL_NAME L\"" + _Option->getProjectNameDll() + L"\"");
}

void VPGBaseGenerationManager::CreateWorkspaceDirectory() const
{
    TRY
        validateOption();
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
        if (_Option->getTemplate() == nullptr || !_Option->getTemplate()->getIsExcludeUnittest())
            checkList.push_back(unittestFolderName);
        
        for (auto path : checkList) {
            std::wstring absPath = vcc::concatPaths({_Workspace, path});
            if (!vcc::isDirectoryExists(absPath)) {
                vcc::createDirectory(absPath);
                vcc::LogService::logInfo(this->getLogConfig().get(), L"", L"Create Directory: " + path);
            }        
        }
    CATCH
}
void VPGBaseGenerationManager::CreateBasicProject() const
{
    TRY
        validateOption();
        this->CreateWorkspaceDirectory();

        std::wstring src = VPGGlobal::getConvertedPath(_Option->getTemplate()->getWorkspace());
        std::wstring dest = _Workspace;
        if (_Option->getIsGit()) {
            vcc::copyFile(vcc::concatPaths({src, L".gitignore"}), vcc::concatPaths({dest, L".gitignore"}), true);
        }
        if (!vcc::isBlank(_Option->getProjectNameExe())) {
            vcc::copyFile(vcc::concatPaths({src, L"main.cpp"}), vcc::concatPaths({dest, L"main.cpp"}), true);
        }
        if (!vcc::isBlank(_Option->getProjectNameDll())) {
            vcc::copyFile(vcc::concatPaths({src, L"DllEntryPoint.cpp"}), vcc::concatPaths({dest, L"DllEntryPoint.cpp"}), true);
            vcc::copyFile(vcc::concatPaths({src, L"DllFunctions.cpp"}), vcc::concatPaths({dest, L"DllFunctions.cpp"}), true);
            vcc::copyFile(vcc::concatPaths({src, L"DllFunctions.h"}), vcc::concatPaths({dest, L"DllFunctions.h"}), true);
        }
        if (_Option->getTemplate() == nullptr || !_Option->getTemplate()->getIsExcludeUnittest()) {
            vcc::copyFile(vcc::concatPaths({src, unittestFolderName, L"gtest_main.cpp"}), vcc::concatPaths({dest, unittestFolderName, L"gtest_main.cpp"}), true);

            if (!vcc::isBlank(_Option->getProjectNameDll())) {
                std::wstring dllUnitTestContent = vcc::readFile(vcc::concatPaths({src, unittestFolderName, L"Dll/dll_test.cpp"}));
                getDLLTestFileContent(dllUnitTestContent);
                vcc::appendFileOneLine(vcc::concatPaths({dest, unittestFolderName, L"Dll/dll_test.cpp"}), dllUnitTestContent, true);
            }
        }
        // Cannot Copy
        vcc::appendFileOneLine(vcc::concatPaths({dest, L"LICENSE"}), L"", true);
        vcc::appendFileOneLine(vcc::concatPaths({dest, L"README.md"}), L"", true);
        vcc::appendFileOneLine(vcc::concatPaths({dest, L"CHANGELOG.md"}), L"", true);

        if (vcc::isFilePresent(vcc::concatPaths({src, MakeFileName}))) {
            // Makefile
            std::wstring makefilePath = vcc::concatPaths({dest, MakeFileName});
            std::wstring makefileContent = vcc::readFile(vcc::concatPaths({src, MakeFileName}));
            vcc::writeFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        }
        
        // .vscode
        std::wstring launchFilePath = vcc::concatPaths({dest,  L".vscode/launch.json"});
        std::wstring launchFileContent = vcc::readFile(vcc::concatPaths({src,  L".vscode/launch.json"}));
        vcc::writeFile(launchFilePath, this->AdjustVSCodeLaunchJson(launchFileContent), true);
        
        std::wstring tasksFilePath = vcc::concatPaths({dest,  L".vscode/tasks.json"});
        std::wstring tasksFileContent = vcc::readFile(vcc::concatPaths({src,  L".vscode/tasks.json"}));
        vcc::writeFile(tasksFilePath, tasksFileContent, true);   
    CATCH
}

void VPGBaseGenerationManager::syncWorkspace(const vcc::LogConfig *logConfig, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters) const
{
    TRY
        std::vector<std::wstring> needToAdd, needToModify, needToDelete;
        TRY
            vcc::getFileDifferenceBetweenWorkspaces(sourceWorkspace, targetWorkspace, needToAdd, needToModify, needToDelete);
        CATCH

        // Delete
        for (auto path : needToDelete) {
            std::wstring targetPath = vcc::concatPaths({targetWorkspace, path});
            if (!std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(targetPath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    targetPath += L"/";
                }                
            }

            if (!includeFileFilters.empty() && !vcc::isPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && vcc::isPathMatchFileFilters(path, excludeFileFilters))
                continue;


            if (vcc::isFile(targetPath)) {
                remove(PATH(targetPath));
                vcc::LogService::logInfo(logConfig, L"", L"Removed Directory: " + targetPath);
            } else {
                std::filesystem::remove_all(PATH(targetPath));
                vcc::LogService::logInfo(logConfig, L"", L"Removed File: " + targetPath);
            }
        }

        // Add
        for (auto path : needToAdd) {
            std::wstring sourcePath = vcc::concatPaths({sourceWorkspace, path});
            std::wstring targetPath = vcc::concatPaths({targetWorkspace, path});
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

            if (!includeFileFilters.empty() && !vcc::isPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && vcc::isPathMatchFileFilters(path, excludeFileFilters))
                continue;

            if (vcc::isFile(sourcePath)) {
                VPGFileSyncService::copyFile(logConfig, VPGFileContentSyncTagMode::Synchronization, sourcePath, targetPath);
            } else {
                vcc::createDirectory(targetPath);
                vcc::LogService::logInfo(logConfig, L"", L"Added Directory: " + targetPath);
            }
        }

        // Modify
        for (auto path : needToModify) {
            if (!includeFileFilters.empty() && !vcc::isPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && vcc::isPathMatchFileFilters(path, excludeFileFilters))
                continue;

            std::wstring sourcePath = vcc::concatPaths({sourceWorkspace, path});
            std::wstring targetPath = vcc::concatPaths({targetWorkspace, path});
            if (!vcc::isFile(sourcePath) || !vcc::isFile(targetPath))
                continue;
            
            // modify file
            VPGFileSyncService::copyFile(logConfig, VPGFileContentSyncTagMode::Synchronization, sourcePath, targetPath);
        }
    CATCH
}

std::wstring VPGBaseGenerationManager::AdjustMakefile(const std::wstring &fileContent) const
{
    validateOption();
    std::wstring result = L"";
    TRY
        auto elements = std::make_shared<vcc::Xml>();
        VPGCodeReader reader(L"#");
        reader.deserialize(fileContent, elements);
        for (std::shared_ptr<vcc::Xml> element : elements->getChildren()) {
            if (element->getName() == L"vcc:name") {
                std::wstring projName = !vcc::isBlank(_Option->getProjectName()) ? (L" " + _Option->getProjectName()) : L"";
                std::wstring dllName = !vcc::isBlank(_Option->getProjectNameDll()) ? (L" " + _Option->getProjectNameDll()) : L"";
                std::wstring exeName = !vcc::isBlank(_Option->getProjectNameExe()) ? (L" " + _Option->getProjectNameExe()) : L"";
                std::wstring IsExcludeUnittest = _Option->getTemplate() != nullptr && _Option->getTemplate()->getIsExcludeUnittest() ? L" Y" : L" N";
        
                result += L"# <vcc:name sync=\"ALERT\" gen=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJ_NAME :=" + projName + L"\r\n";
                result += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
                result += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
                result += L"IS_EXCLUDE_UNITTEST :=" + IsExcludeUnittest + L"\r\n";
                result += L"# </vcc:name>";
            } else if (element->getName() == L"vcc:export") {
                std::wstring exportDllDirWindow = L"";
                std::wstring exportExeDirWindow = L"";
                std::wstring exportExternalLibDirWindow = L"";
                std::wstring exportDllDirLinux = L"";
                std::wstring exportExeDirLinux = L"";
                std::wstring exportExternalLibDirLinux = L"";

                for (auto const &exportOption : _Option->getExports()) {
                    if (vcc::isBlank(exportOption->getWorkspace()))
                        continue;
                    std::wstring workspace = exportOption->getWorkspace(); //isAbsolutePath(exportOption->getWorkspace()) ? exportOption->getWorkspace() : vcc::ConcatPaths({ _Workspace, exportOption->getWorkspace() });

                    if (!vcc::isBlank(exportOption->getExportDirectoryDll())) {
                        exportDllDirWindow += L" " + vcc::getWindowPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryDll() }));
                        exportDllDirLinux += L" " + vcc::getLinuxPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryDll() }));
                        if (exportOption->getIsExportExternalLib()) {
                            exportExternalLibDirWindow += L" " + vcc::getWindowPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryDll() }));
                            exportExternalLibDirLinux += L" " + vcc::getLinuxPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryDll() }));
                        }
                    }
                    if (!vcc::isBlank(exportOption->getExportDirectoryExe())) {
                        exportExeDirWindow += L" " + vcc::getWindowPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryExe() }));
                        exportExeDirLinux += L" " + vcc::getLinuxPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryExe() }));
                        if (exportOption->getIsExportExternalLib()) {
                            exportExternalLibDirWindow += L" " + vcc::getWindowPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryExe() }));
                            exportExternalLibDirLinux += L" " + vcc::getLinuxPath(vcc::concatPaths({ workspace, exportOption->getExportDirectoryExe() }));                            
                        }
                    }
                }
                vcc::replaceAll(exportDllDirWindow, L"\\", L"\\\\");
                vcc::replaceAll(exportExeDirWindow, L"\\", L"\\\\");
                vcc::replaceAll(exportExternalLibDirWindow, L"\\", L"\\\\");

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
                result += element->getFullText();
        }
    CATCH
    return result;
}

std::wstring VPGBaseGenerationManager::AdjustVSCodeLaunchJson(const std::wstring &fileContent) const
{
    TRY
        std::wstring programPath = L"${workspaceFolder}/bin/Debug/unittest";
        if (_Option->getTemplate() != nullptr && _Option->getTemplate()->getIsExcludeUnittest() && !_Option->getProjectNameExe().empty()) {
            std::wstring projectName = _Option->getProjectNameExe();
            #ifdef __WIN32
            projectName += L".exe";
            #endif
            programPath = L"${workspaceFolder}/bin/Debug/" + projectName;
        }

        auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
        jsonBuilder->setIsBeautify(true);
        jsonBuilder->setIndent(L"  ");

        auto json = std::make_shared<vcc::Json>();
        jsonBuilder->deserialize(fileContent, json);
        for (std::shared_ptr<vcc::Json> element : json->getArray(L"configurations")) {
            for (std::shared_ptr<vcc::Json> arrayElement : element->getJsonInternalArray()) {
                if (arrayElement->isContainKey(L"program"))
                    arrayElement->setString(L"program", programPath);
            }
        }
        return jsonBuilder->serialize(json.get());
    CATCH
    return fileContent;
}
