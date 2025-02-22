#include "vpg_vcc_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "platform_type.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_global.hpp"

using namespace vcc;

#define CLASS_ID L"VPGVccGenerationManager"

std::vector<std::wstring> VPGVccGenerationManager::GetUpdateList() const
{
    std::vector<std::wstring> result;
    // VCC Core
    result.push_back(L"include/external/vcc/LICENSE");
    result.push_back(L"include/external/vcc/core/*");
    result.push_back(L"src/external/vcc/core/*");
    
    // Need to tmp create first, then override by Generate Mode
    // Otherwise, cannot be compiled
    // application
    
    // type
    result.push_back(ConcatPaths({_Option->GetExceptionTypeDirectory(), L"exception_type.hpp"}));
    result.push_back(ConcatPaths({_Option->GetObjectTypeDirectory(), L"object_type.hpp"}));
    
    // application
    if (!IsBlank(_Option->GetApplicationDirectoryHpp()))
        result.push_back(ConcatPaths({_Option->GetApplicationDirectoryHpp(), L"application.hpp"}));
    if (!IsBlank(_Option->GetApplicationDirectoryCpp()))
        result.push_back(ConcatPaths({_Option->GetApplicationDirectoryCpp(), L"application.cpp"}));

    // factory
    if (!IsBlank(_Option->GetObjectFactoryDirectoryHpp()))
        result.push_back(ConcatPaths({_Option->GetObjectFactoryDirectoryHpp(), L"object_factory.hpp"}));
    if (!IsBlank(_Option->GetObjectFactoryDirectoryCpp()))
        result.push_back(ConcatPaths({_Option->GetObjectFactoryDirectoryCpp(), L"object_factory.cpp"}));
    if (!IsBlank(_Option->GetPropertyAccessorFactoryDirectoryHpp()))
        result.push_back(ConcatPaths({_Option->GetPropertyAccessorFactoryDirectoryHpp(), L"property_accessor_factory.hpp"}));
    if (!IsBlank(_Option->GetPropertyAccessorFactoryDirectoryCpp()))
        result.push_back(ConcatPaths({_Option->GetPropertyAccessorFactoryDirectoryCpp(), L"property_accessor_factory.cpp"}));

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

void VPGVccGenerationManager::CreateVccJson(bool isNew) const
{
    TRY
        auto jsonBuilder = std::make_unique<JsonBuilder>();
        jsonBuilder->SetIsBeautify(true);
        _Option->SetVersion(VPGGlobal::GetVersion());
        if (isNew)
            _Option->InsertExports(std::make_shared<VPGGenerationOptionExport>());
        WriteFile(ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()}), _Option->SerializeJson(jsonBuilder.get()), true);
    CATCH
}

void VPGVccGenerationManager::ReadVccJson() const
{
    TRY
        std::wstring fileContent = ReadFile(ConcatPaths({_Workspace, VPGGlobal::GetVccJsonFileName()}));
        auto jsonBuilder = std::make_unique<JsonBuilder>();
        auto json = std::make_shared<Json>();
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
        CreateVccJson(true);
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
        if (!IsFilePresent(ConcatPaths({dest, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + ConcatPaths({dest, MakeFileName}));
        
        // Makefile
        std::wstring makefilePath = ConcatPaths({dest, MakeFileName});
        std::wstring makefileContent = ReadFile(ConcatPaths({dest, MakeFileName}));
        WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        // Create Json file at the end to force override
        CreateVccJson(false);
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Done");        
    CATCH
}

void VPGVccGenerationManager::Generate() const
{
    TRY
        ReadVccJson();
        
        // Update Makefile
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Update Project according to vcc.json");
        if (!IsFilePresent(ConcatPaths({_Workspace, MakeFileName})))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot find " + ConcatPaths({_Workspace, MakeFileName}));
        std::wstring makefilePath = ConcatPaths({_Workspace, MakeFileName});
        std::wstring makefileContent = ReadFile(ConcatPaths({_Workspace, MakeFileName}));
        WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        
        auto manager = std::make_unique<VPGFileGenerationManager>(this->_LogConfig, _Workspace);
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Generate Project ...");
        manager->GernerateProperty(_LogConfig.get(), _Option.get());
        LogService::LogInfo(this->_LogConfig.get(), CLASS_ID, L"Done");
    CATCH
}