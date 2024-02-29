#include "vpg_vcc_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"

#include "vpg_code_reader.hpp"
#include "vpg_directory_sync_service.hpp"

using namespace vcc;

#define CLASS_ID L"VPGVccGenerationManager"

void VPGVccGenerationManager::Add()
{
    TRY_CATCH(
        this->CreateBasicProject();
        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Copy Project to " + dest);
        CopyDirectoryOption copyDirectoryOption;
        copyDirectoryOption.SetIsForce(true);
        copyDirectoryOption.SetIsRecursive(true);
        copyDirectoryOption.InsertIncludeFileFilters(L"include/External/VCC/LICENSE");
        copyDirectoryOption.InsertIncludeFileFilters(L"include/External/VCC/Core/*");
        copyDirectoryOption.InsertIncludeFileFilters(L"src/External/VCC/Core/*");
        bool isExcludeUnitTest = IsBlank(_Option->GetProjectNameGtest());
        if (!isExcludeUnitTest) {
            copyDirectoryOption.InsertIncludeFileFilters(_Option->GetProjectNameGtest() + L"/External/VCC/Core/*");
            if (!IsBlank(_Option->GetProjectNameDLL())) {
                copyDirectoryOption.InsertIncludeFileFilters(_Option->GetProjectNameGtest() + L"/dllTest/*");
            }
            copyDirectoryOption.InsertIncludeFileFilters(_Option->GetProjectNameGtest() + L"/gtest_main.cpp");
        }
        
        // plugins
        for (auto const &str : _Option->GetPlugins()) {
            copyDirectoryOption.InsertExcludeFileFilters(ConcatPath(ConcatPath(L"include/External/", str), L"*"));
            copyDirectoryOption.InsertExcludeFileFilters(ConcatPath(ConcatPath(L"src/External/", str), L"*"));
            if (!isExcludeUnitTest) {
                copyDirectoryOption.InsertExcludeFileFilters(ConcatPath(ConcatPath(_Option->GetProjectNameGtest() + L"/External/", str), L"*"));
            }
        }
        CopyDirectory(src, dest, &copyDirectoryOption);
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Done.");
    )
}

void VPGVccGenerationManager::Update()
{
}

void VPGVccGenerationManager::Generate()
{
}