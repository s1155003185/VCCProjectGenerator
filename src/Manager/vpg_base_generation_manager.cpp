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
        json->AddBool(L"IsGit", _IsGit);

        json->AddString(L"ProjectPrefix", _ProjectPrefix);

        json->AddString(L"ProjectName", _ProjectName);
        json->AddString(L"ProjectNameDLL", _ProjectNameDLL);
        json->AddString(L"ProjectNameEXE", _ProjectNameEXE);
        json->AddString(L"ProjectNameGtest", _ProjectNameGtest);

        json->AddBool(L"IsExcludeVCCUnitTest", _IsExcludeVCCUnitTest);

        json->AddString(L"ActionTypeDirectory", _ActionTypeDirectory);
        json->AddString(L"ExceptionTypeDirectory", _ExceptionTypeDirectory);
        json->AddString(L"ManagerTypeDirectory", _ManagerTypeDirectory);
        json->AddString(L"ObjectTypeDirectory", _ObjectTypeDirectory);


        json->AddString(L"TypeWorkspace", _TypeWorkspace);
        json->AddString(L"ObjectHppDirectory", _ObjectHppDirectory);
        json->AddString(L"ObjectTypeHppDirectory", _ObjectTypeHppDirectory);
        json->AddString(L"PropertyAccessorHppDirectory", _PropertyAccessorHppDirectory);
        json->AddString(L"PropertyAccessorCppDirectory", _PropertyAccessorCppDirectory);

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
        if (json->IsContainKey(L"ProjectNameDLL"))
            this->SetProjectNameDLL(json->GetString(L"ProjectNameDLL"));
        if (json->IsContainKey(L"ProjectNameEXE"))
            this->SetProjectNameEXE(json->GetString(L"ProjectNameEXE"));
        if (json->IsContainKey(L"ProjectNameGtest"))
            this->SetProjectNameGtest(json->GetString(L"ProjectNameGtest"));

        if (json->IsContainKey(L"IsExcludeVCCUnitTest"))
            this->SetIsExcludeVCCUnitTest(json->GetBool(L"IsExcludeVCCUnitTest"));

        if (json->IsContainKey(L"ActionTypeDirectory"))
            this->SetActionTypeDirectory(json->GetString(L"ActionTypeDirectory"));
        if (json->IsContainKey(L"ExceptionTypeDirectory"))
            this->SetExceptionTypeDirectory(json->GetString(L"ExceptionTypeDirectory"));
        if (json->IsContainKey(L"ManagerTypeDirectory"))
            this->SetManagerTypeDirectory(json->GetString(L"ManagerTypeDirectory"));
        if (json->IsContainKey(L"ObjectTypeDirectory"))
            this->SetObjectTypeDirectory(json->GetString(L"ObjectTypeDirectory"));

        if (json->IsContainKey(L"TypeWorkspace"))
            this->SetTypeWorkspace(json->GetString(L"TypeWorkspace"));
        if (json->IsContainKey(L"ObjectHppDirectory"))
            this->SetObjectHppDirectory(json->GetString(L"ObjectHppDirectory"));
        if (json->IsContainKey(L"ObjectTypeHppDirectory"))
            this->SetObjectTypeHppDirectory(json->GetString(L"ObjectTypeHppDirectory"));
        if (json->IsContainKey(L"PropertyAccessorHppDirectory"))
            this->SetPropertyAccessorHppDirectory(json->GetString(L"PropertyAccessorHppDirectory"));
        if (json->IsContainKey(L"PropertyAccessorCppDirectory"))
            this->SetPropertyAccessorCppDirectory(json->GetString(L"PropertyAccessorCppDirectory"));

        if (json->IsContainKey(L"Plugins")) {
            auto plugins = json->GetArray(L"Plugins");
            for (auto const &plugin : plugins) {
                this->InsertPlugins(plugin->GetValue());
            }
        }
    )
}
