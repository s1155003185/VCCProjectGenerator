#pragma once

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "base_json_object.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "i_vpg_generation_manager.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

const std::wstring MakeFileName = L"Makefile";
const std::wstring unittestFolderName = L"unittest";

template <typename Derived>
class VPGBaseGenerationManager : public BaseManager<Derived>, public IVPGGenerationManager
{
    GET_SPTR(LogProperty, LogProperty);
    GET_SPTR(VPGGenerationOption, Option);
    
    protected:
        void ValidateOption() const;
    public:
        VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option) : BaseManager<Derived>(logProperty)
        { 
            assert(option != nullptr);
            this->_Option = option;
        }

        virtual ~VPGBaseGenerationManager() {}

        void GetDLLTestFileContent(std::wstring &fileContent) const;
        
        void CreateWorkspaceDirectory() const;
        void CreateBasicProject() const;
        std::wstring AdjustMakefile(const std::wstring &fileContent) const;
        std::wstring AdjustVSCodeLaunchJson(const std::wstring &fileContent) const;

        void SyncWorkspace(const LogProperty *logProperty, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters) const;
        
        virtual void Add() const override = 0;

        virtual void Update() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
        }

        virtual void Generate() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
        }
};

template <typename Derived>
void VPGBaseGenerationManager<Derived>::ValidateOption() const
{
    TRY
        if (IsBlank(_Option->GetWorkspaceSource()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace Source is emtpy.");
        if (IsBlank(_Option->GetWorkspaceDestination()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace Distination is emtpy.");
        if (IsBlank(_Option->GetProjectName()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Project Name is emtpy.");
        if (IsBlank(_Option->GetProjectNameDll()) && IsBlank(_Option->GetProjectNameExe()))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Both DLL name and EXE name both empty.");
    CATCH
}

template <typename Derived>
void VPGBaseGenerationManager<Derived>::GetDLLTestFileContent(std::wstring &fileContent) const
{
    ReplaceRegex(fileContent, L"#define DLL_NAME L\"([^\"]*)\"", L"#define DLL_NAME L\"" + _Option->GetProjectNameDll() + L"\"");
}

template <typename Derived>
void VPGBaseGenerationManager<Derived>::CreateWorkspaceDirectory() const
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
        
        std::wstring workspace = _Option->GetWorkspaceDestination();
        for (auto path : checkList) {
            std::wstring absPath = ConcatPaths({workspace, path});
            if (!IsDirectoryExists(absPath)) {
                CreateDirectory(absPath);
                LogService::LogInfo(this->GetLogProperty().get(), L"", L"Create Directory: " + path);
            }        
        }
    CATCH
}

template <typename Derived>
void VPGBaseGenerationManager<Derived>::CreateBasicProject() const
{
    TRY
        ValidateOption();
        this->CreateWorkspaceDirectory();

        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
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

        if (IsFileExists(ConcatPaths({src, MakeFileName}))) {
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

template <typename Derived>
void VPGBaseGenerationManager<Derived>::SyncWorkspace(const LogProperty *logProperty, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
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
                LogService::LogInfo(logProperty, L"", L"Removed Directory: " + targetPath);
            } else {
                std::filesystem::remove_all(PATH(targetPath));
                LogService::LogInfo(logProperty, L"", L"Removed File: " + targetPath);
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
                VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
            } else {
                CreateDirectory(targetPath);
                LogService::LogInfo(logProperty, L"", L"Added Directory: " + targetPath);
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
            VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
        }
    CATCH
}

template <typename Derived>
std::wstring VPGBaseGenerationManager<Derived>::AdjustMakefile(const std::wstring &fileContent) const
{
    ValidateOption();
    std::wstring result = L"";
    TRY
        DECLARE_SPTR(Xml, elements);
        VPGCodeReader reader(L"#");
        reader.Deserialize(fileContent, elements);
        for (std::shared_ptr<vcc::Xml> element : elements->GetChildren()) {
            if (element->GetName() == L"vcc:name") {
                std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
                std::wstring dllName = !IsBlank(_Option->GetProjectNameDll()) ? (L" " + _Option->GetProjectNameDll()) : L"";
                std::wstring exeName = !IsBlank(_Option->GetProjectNameExe()) ? (L" " + _Option->GetProjectNameExe()) : L"";
                std::wstring IsExcludeUnittest = _Option->GetIsExcludeUnittest() ? L" Y" : L" N";
        
                result += L"# <vcc:name sync=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJ_NAME :=" + projName + L"\r\n";
                result += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
                result += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
                result += L"IS_EXCLUDE_UNITTEST :=" + IsExcludeUnittest + L"\r\n";
                result += L"# </vcc:name>";
            } else
                result += element->GetFullText();
        }
    CATCH
    return result;
}

template <typename Derived>
std::wstring VPGBaseGenerationManager<Derived>::AdjustVSCodeLaunchJson(const std::wstring &fileContent) const
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

        DECLARE_UPTR(JsonBuilder, jsonBuilder);
        jsonBuilder->SetIsBeautify(true);
        jsonBuilder->SetIndent(L"  ");

        DECLARE_SPTR(Json, json);
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
