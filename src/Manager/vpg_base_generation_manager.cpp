#include "vpg_base_generation_manager.hpp"

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

std::wstring VPGGenerationOption::SerializeJson(const IDocumentBuilder *builder)
{
    TRY_CATCH(
        DECLARE_UPTR(Json, json);
        json->AddString(L"Version", _Version);
        
        json->AddString(L"ProjectType", L"VCCModule");
        json->AddString(L"WorkspaceSourceGitUrl", _WorkspaceSourceGitUrl);
        json->AddString(L"WorkspaceSource", _WorkspaceSource);
        json->AddString(L"WorkspaceDestination", _WorkspaceDestination);

        // --------------------------------------------------
        // Config
        // --------------------------------------------------
        json->AddString(L"ProjectPrefix", _ProjectPrefix);

        json->AddString(L"ProjectName", _ProjectName);
        json->AddString(L"ProjectNameDll", _ProjectNameDll);
        json->AddString(L"ProjectNameExe", _ProjectNameExe);
        json->AddBool(L"IsGit", _IsGit);

        json->AddBool(L"IsExcludeUnittest", _IsExcludeUnittest);
        json->AddBool(L"IsExcludeVCCUnitTest", _IsExcludeVCCUnitTest);

        json->AddString(L"TypeWorkspace", _TypeWorkspace);

        json->AddString(L"ActionTypeDirectory", _ActionTypeDirectory);
        json->AddString(L"ExceptionTypeDirectory", _ExceptionTypeDirectory);
        json->AddString(L"ManagerTypeDirectory", _ManagerTypeDirectory);
        json->AddString(L"ObjectTypeDirectory", _ObjectTypeDirectory);

        json->AddString(L"ModelDirectory", _ModelDirectory);
        json->AddString(L"PropertyAccessorDirectoryHpp", _PropertyAccessorDirectoryHpp);
        json->AddString(L"PropertyAccessorDirectoryCpp", _PropertyAccessorDirectoryCpp);

        DECLARE_SPTR(Json, plugins);
        json->AddArray(L"Plugins", plugins);
        for (auto const &plugin : _Plugins) {
            plugins->AddArrayString(plugin);
        }
        
        return builder->Serialize(json.get());
    )
    return L"";
}

void VPGGenerationOption::DeserializeJson(std::shared_ptr<IDocument> document)
{
    TRY_CATCH(
        std::shared_ptr<Json> json = dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);

        if (json->IsContainKey(L"Version"))
            this->SetVersion(json->GetString(L"Version"));
        if (json->IsContainKey(L"IsGit"))
            this->SetIsGit(json->GetBool(L"IsGit"));
        if (json->IsContainKey(L"ProjectPrefix"))
            this->SetProjectPrefix(json->GetString(L"ProjectPrefix"));


        if (json->IsContainKey(L"ProjectName"))
            this->SetProjectName(json->GetString(L"ProjectName"));
        if (json->IsContainKey(L"ProjectNameDll"))
            this->SetProjectNameDll(json->GetString(L"ProjectNameDll"));
        if (json->IsContainKey(L"ProjectNameExe"))
            this->SetProjectNameExe(json->GetString(L"ProjectNameExe"));
            
        if (json->IsContainKey(L"IsExcludeUnittest"))
            this->SetIsExcludeUnittest(json->GetBool(L"IsExcludeUnittest"));
        if (json->IsContainKey(L"IsExcludeVCCUnitTest"))
            this->SetIsExcludeVCCUnitTest(json->GetBool(L"IsExcludeVCCUnitTest"));

        if (json->IsContainKey(L"TypeWorkspace"))
            this->SetTypeWorkspace(json->GetString(L"TypeWorkspace"));

        if (json->IsContainKey(L"ActionTypeDirectory"))
            this->SetActionTypeDirectory(json->GetString(L"ActionTypeDirectory"));
        if (json->IsContainKey(L"ExceptionTypeDirectory"))
            this->SetExceptionTypeDirectory(json->GetString(L"ExceptionTypeDirectory"));
        if (json->IsContainKey(L"ManagerTypeDirectory"))
            this->SetManagerTypeDirectory(json->GetString(L"ManagerTypeDirectory"));
        if (json->IsContainKey(L"ObjectTypeDirectory"))
            this->SetObjectTypeDirectory(json->GetString(L"ObjectTypeDirectory"));

        if (json->IsContainKey(L"ModelDirectory"))
            this->SetModelDirectory(json->GetString(L"ModelDirectory"));
        if (json->IsContainKey(L"PropertyAccessorDirectoryHpp"))
            this->SetPropertyAccessorDirectoryHpp(json->GetString(L"PropertyAccessorDirectoryHpp"));
        if (json->IsContainKey(L"PropertyAccessorDirectoryCpp"))
            this->SetPropertyAccessorDirectoryCpp(json->GetString(L"PropertyAccessorDirectoryCpp"));

        if (json->IsContainKey(L"Plugins")) {
            auto plugins = json->GetArray(L"Plugins");
            for (auto const &plugin : plugins) {
                this->InsertPlugins(plugin->GetJsonInternalValue());
            }
        }
    )
}
