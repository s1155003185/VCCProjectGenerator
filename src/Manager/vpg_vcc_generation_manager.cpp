#include "vpg_vcc_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"
#include "platform_type.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_global.hpp"

using namespace vcc;

#define CLASS_ID L"VPGVccGenerationManager"

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateList() const
{
    std::vector<std::wstring> result;
    // basic file
    result.push_back(ConcatPaths({_Option->GetActionTypeDirectory(), L"action_type.hpp"}));
    result.push_back(ConcatPaths({_Option->GetExceptionTypeDirectory(), L"exception_type.hpp"}));
    result.push_back(ConcatPaths({_Option->GetManagerTypeDirectory(), L"manager_type.hpp"}));
    result.push_back(ConcatPaths({_Option->GetObjectTypeDirectory(), L"object_type.hpp"}));
    
    // VCC Core
    result.push_back(L"include/External/VCC/LICENSE");
    result.push_back(L"include/External/VCC/Core/*");
    result.push_back(L"src/External/VCC/Core/*");
    
    // plugins
    for (auto const &str : _Option->GetPlugins()) {
        result.push_back(ConcatPaths({L"include/External/", str, L"*"}));
        result.push_back(ConcatPaths({L"src/External/", str, L"*"}));
    }
    return result;
}

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateUnitTestList() const
{
    std::vector<std::wstring> result;
    if (!_Option->GetIsExcludeVCCUnitTest())
        result.push_back(L"External/VCC/Core/*");

    for (auto const &str : _Option->GetPlugins()) {
        if (!_Option->GetIsExcludeUnittest()) {
            if (!(str.starts_with(L"VCC") && _Option->GetIsExcludeVCCUnitTest()))
                result.push_back(ConcatPaths({L"External/", str, L"*"}));
        }
    }
    return result;    
}

void VPGVccGenerationManager::CreateVccJson() const
{
    TRY_CATCH() {
        DECLARE_UPTR(JsonBuilder, jsonBuilder);
        jsonBuilder->SetIsBeautify(true);

        // Vector cannot be inizalize when create, initialize here
        // if (_Option->GetPlatforms().empty()) {
        //     // initialize include paths
        //     DECLARE_SPTR(VPGGenerationOptionPlatform, platformWin);
        //     platformWin->SetPlatform(PlatformType::Window);
        //     platformWin->InsertIncludePaths(VPGGlobal::GetCppDefaultIncludePathWindow());
        //     _Option->InsertPlatforms(platformWin);

        //     DECLARE_SPTR(VPGGenerationOptionPlatform, platformLinux);
        //     platformLinux->SetPlatform(PlatformType::Linux);
        //     platformLinux->InsertIncludePaths(VPGGlobal::GetCppDefaultIncludePathLinux());
        //     _Option->InsertPlatforms(platformLinux);

        //     DECLARE_SPTR(VPGGenerationOptionPlatform, platformMacOS);
        //     platformMacOS->SetPlatform(PlatformType::MacOs);
        //     platformMacOS->InsertIncludePaths(VPGGlobal::GetCppDefaultIncludePathMacOs());
        //     _Option->InsertPlatforms(platformMacOS);
        // }
        WriteFile(ConcatPaths({_Option->GetWorkspaceDestination(), VPGGlobal::GetVccJsonFileName()}), _Option->SerializeJson(jsonBuilder.get()), true);
    }
}

void VPGVccGenerationManager::ReadVccJson() const
{
    TRY_CATCH() {
        std::wstring fileContent = ReadFile(ConcatPaths({_Option->GetWorkspaceDestination(), VPGGlobal::GetVccJsonFileName()}));
        DECLARE_UPTR(JsonBuilder, jsonBuilder);
        DECLARE_SPTR(Json, json);
        jsonBuilder->Deserialize(fileContent, json);
        _Option->DeserializeJson(json);
    }
}

void VPGVccGenerationManager::Add() const
{
    TRY_CATCH() {
        VPGBaseGenerationManager::CreateBasicProject();
        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Copy Project to " + dest + L" ...");
        CopyDirectoryOption copyDirectoryOption;
        copyDirectoryOption.SetIsForce(true);
        copyDirectoryOption.SetIsRecursive(true);
        
        for (auto const &str : GetUpdateList())
            copyDirectoryOption.InsertIncludeFileFilters(str);
        CopyDirectory(src, dest, &copyDirectoryOption);

        // handle unittest in next loop as unit test name can be changed
        if (!_Option->GetIsExcludeUnittest()) {
            copyDirectoryOption.ClearIncludeFileFilters();
            for (auto const &str : GetUpdateUnitTestList())
                copyDirectoryOption.InsertIncludeFileFilters(str);
            if (!copyDirectoryOption.GetIncludeFileFilters().empty())
                CopyDirectory(ConcatPaths({src, unittestFolderName}), ConcatPaths({dest, unittestFolderName}), &copyDirectoryOption);
        }
        
        // Create Json file at the end to force override
        CreateVccJson();
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Done");
    }
}

void VPGVccGenerationManager::Update() const
{
    TRY_CATCH() {
        ReadVccJson();

        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Sync Project ...");
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"From " + src);
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"To " + dest);

        SyncWorkspace(this->_LogProperty.get(), src, dest, GetUpdateList(), {});
        
        if (!_Option->GetIsExcludeUnittest()) {
            auto list = GetUpdateUnitTestList();
            if (!list.empty())
                SyncWorkspace(this->_LogProperty.get(), ConcatPaths({src, unittestFolderName}), ConcatPaths({dest, unittestFolderName}), list, {});
        }

        // Update Makefile and unittest
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!IsFileExists(ConcatPaths({dest, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + ConcatPaths({dest, MakeFileName}));
        
        // Makefile
        std::wstring makefilePath = ConcatPaths({dest, MakeFileName});
        std::wstring makefileContent = ReadFile(ConcatPaths({dest, MakeFileName}));
        WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        // Create Json file at the end to force override
        CreateVccJson();
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Done");        
    }
}

void VPGVccGenerationManager::Generate() const
{
    TRY_CATCH() {
        ReadVccJson();
        
        DECLARE_UPTR(VPGFileGenerationManager, manager, this->_LogProperty);
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Generate Project ...");
        manager->GernerateProperty(_LogProperty.get(), _Option->GetProjectPrefix(), _Option->GetWorkspaceDestination(), _Option->GetTypeWorkspace(),
            _Option->GetObjectTypeDirectory(), _Option->GetModelDirectory(), _Option->GetPropertyAccessorDirectoryHpp(), _Option->GetPropertyAccessorDirectoryCpp());
        LogService::LogInfo(this->_LogProperty.get(), CLASS_ID, L"Done");
    }
}