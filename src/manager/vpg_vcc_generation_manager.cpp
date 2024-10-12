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
    result.push_back(L"include/external/vcc/LICENSE");
    result.push_back(L"include/external/vcc/core/*");
    result.push_back(L"src/external/vcc/core/*");
    
    // plugins
    for (auto const &str : _Option->GetPlugins()) {
        result.push_back(ConcatPaths({L"include/external/", str, L"*"}));
        result.push_back(ConcatPaths({L"src/external/", str, L"*"}));
    }
    return result;
}

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateUnitTestList() const
{
    std::vector<std::wstring> result;
    if (!_Option->GetIsExcludeVCCUnitTest())
        result.push_back(L"external/vcc/core/*");

    for (auto const &str : _Option->GetPlugins()) {
        if (!_Option->GetIsExcludeUnittest()) {
            if (!(str.starts_with(L"vcc") && _Option->GetIsExcludeVCCUnitTest()))
                result.push_back(ConcatPaths({L"external/", str, L"*"}));
        }
    }
    return result;    
}

void VPGVccGenerationManager::CreateVccJson() const
{
    TRY
        DECLARE_UPTR(JsonBuilder, jsonBuilder);
        jsonBuilder->SetIsBeautify(true);
        _Option->SetVersion(VPGGlobal::GetVersion());
        WriteFile(ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()}), _Option->SerializeJson(jsonBuilder.get()), true);
    CATCH
}

void VPGVccGenerationManager::ReadVccJson() const
{
    TRY
        std::wstring fileContent = ReadFile(ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()}));
        DECLARE_UPTR(JsonBuilder, jsonBuilder);
        DECLARE_SPTR(Json, json);
        jsonBuilder->Deserialize(fileContent, json);
        _Option->DeserializeJson(json);
    CATCH
}

void VPGVccGenerationManager::Add() const
{
    TRY
        VPGBaseGenerationManager::CreateBasicProject();
        std::wstring src = VPGGlobal::GetConvertedPath(_Option->GetTemplateWorkspace());
        std::wstring dest = _Workspace;
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Copy Project to " + dest + L" ...");
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
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Done");
    CATCH
}

void VPGVccGenerationManager::Update() const
{
    TRY
        ReadVccJson();

        std::wstring src = VPGGlobal::GetConvertedPath(_Option->GetTemplateWorkspace());
        std::wstring dest = _Workspace;
        
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Sync Project ...");
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"From " + src);
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"To " + dest);

        SyncWorkspace(this->_LogConfig.get(), src, dest, GetUpdateList(), {});
        
        if (!_Option->GetIsExcludeUnittest()) {
            auto list = GetUpdateUnitTestList();
            if (!list.empty())
                SyncWorkspace(this->_LogConfig.get(), ConcatPaths({src, unittestFolderName}), ConcatPaths({dest, unittestFolderName}), list, {});
        }

        // Update Makefile and unittest
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!IsFileExists(ConcatPaths({dest, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + ConcatPaths({dest, MakeFileName}));
        
        // Makefile
        std::wstring makefilePath = ConcatPaths({dest, MakeFileName});
        std::wstring makefileContent = ReadFile(ConcatPaths({dest, MakeFileName}));
        WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        // Create Json file at the end to force override
        CreateVccJson();
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Done");        
    CATCH
}

void VPGVccGenerationManager::Generate() const
{
    TRY
        ReadVccJson();
        
        // Update Makefile
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!IsFileExists(ConcatPaths({_Workspace, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + ConcatPaths({_Workspace, MakeFileName}));
        std::wstring makefilePath = ConcatPaths({_Workspace, MakeFileName});
        std::wstring makefileContent = ReadFile(ConcatPaths({_Workspace, MakeFileName}));
        WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        DECLARE_UPTR(VPGFileGenerationManager, manager, this->_LogConfig, _Workspace);
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Generate Project ...");
        manager->GernerateProperty(_LogConfig.get(), _Option.get());
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Done");
    CATCH
}