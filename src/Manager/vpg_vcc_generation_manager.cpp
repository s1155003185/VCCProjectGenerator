#include "vpg_vcc_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_service.hpp"

using namespace vcc;

#define CLASS_ID L"VPGVccGenerationManager"

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateList()
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

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateUnitTestList()
{
    std::vector<std::wstring> result;
    if (!_Option->GetIsExcludeVCCUnitTest())
        result.push_back(L"External/VCC/Core/*");

    for (auto const &str : _Option->GetPlugins()) {
        if (!IsBlank(_Option->GetProjectNameGtest())) {
            if (!(str.starts_with(L"VCC") && _Option->GetIsExcludeVCCUnitTest()))
                result.push_back(ConcatPaths({L"External/", str, L"*"}));
        }
    }
    return result;    
}

void VPGVccGenerationManager::Add()
{
    TRY_CATCH(
        this->CreateBasicProject();
        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Copy Project to " + dest + L" ...");
        CopyDirectoryOption copyDirectoryOption;
        copyDirectoryOption.SetIsForce(true);
        copyDirectoryOption.SetIsRecursive(true);
        
        for (auto const &str : GetUpdateList())
            copyDirectoryOption.InsertIncludeFileFilters(str);
        CopyDirectory(src, dest, &copyDirectoryOption);

        // handle unittest in next loop as unit test name can be changed
        if (!IsBlank(_Option->GetProjectNameGtest())) {
            copyDirectoryOption.ClearIncludeFileFilters();
            for (auto const &str : GetUpdateUnitTestList())
                copyDirectoryOption.InsertIncludeFileFilters(str);
            if (!copyDirectoryOption.GetIncludeFileFilters().empty())
                CopyDirectory(ConcatPaths({src, L"unittest"}), ConcatPaths({dest, _Option->GetProjectNameGtest()}), &copyDirectoryOption);
        }

        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Done");
    )
}

void VPGVccGenerationManager::Update()
{
    TRY_CATCH(
        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Sync Project ...");
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"From " + src);
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"To " + dest);

        SyncWorkspace(this->GetLogProperty().get(), src, dest, GetUpdateList(), {});
        
        if (!IsBlank(_Option->GetProjectNameGtest())) {
            auto list = GetUpdateUnitTestList();
            if (!list.empty())
                SyncWorkspace(this->GetLogProperty().get(), ConcatPaths({src, L"unittest"}), ConcatPaths({dest, _Option->GetProjectNameGtest()}), list, {});
        }
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Done");        
    )
}

void VPGVccGenerationManager::Generate()
{
    TRY_CATCH(
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Generate Project ...");
        VPGFileGenerationService::GernerateProperty(_LogProperty.get(), _Option->GetProjectPrefix(), _Option->GetWorkspaceDestination(), _Option->GetTypeWorkspace(),
            _Option->GetObjectTypeHppDirectory(), _Option->GetObjectHppDirectory(), _Option->GetPropertyAccessorHppDirectory(), _Option->GetPropertyAccessorCppDirectory());
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Done");
    )
}