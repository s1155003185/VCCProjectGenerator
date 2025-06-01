// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_workspace_form.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "base_form.hpp"
#include "exception_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "i_result.hpp"
#include "json.hpp"
#include "number_helper.hpp"
#include "string_helper.hpp"
#include "vpg_git_form.hpp"
#include "vpg_workspace_form_property.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

VPGWorkspaceForm::VPGWorkspaceForm() : vcc::BaseForm()
{
    TRY
        _ObjectType = ObjectType::WorkspaceForm;
        Initialize();
    CATCH
}

std::shared_ptr<vcc::IObject> VPGWorkspaceForm::Clone() const
{
    auto obj = std::make_shared<VPGWorkspaceForm>(*this);
    obj->CloneGitForms(this->_GitForms);
    return obj;
}

std::shared_ptr<vcc::Json> VPGWorkspaceForm::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Name
        json->AddString(vcc::ConvertNamingStyle(L"Name", vcc::NamingStyle::PascalCase, namestyle), GetName());
        // TabOrder
        json->AddInt(vcc::ConvertNamingStyle(L"TabOrder", vcc::NamingStyle::PascalCase, namestyle), GetTabOrder());
        // GitForms
        auto tmpGitForms = std::make_shared<vcc::Json>();
        json->AddArray(vcc::ConvertNamingStyle(L"GitForms", vcc::NamingStyle::PascalCase, namestyle), tmpGitForms);
        for (auto const &element : GetGitForms()) {
            tmpGitForms->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGWorkspaceForm::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Name
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"Name", namestyle, vcc::NamingStyle::PascalCase)))
            SetName(json->GetString(vcc::ConvertNamingStyle(L"Name", namestyle, vcc::NamingStyle::PascalCase)));
        // TabOrder
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"TabOrder", namestyle, vcc::NamingStyle::PascalCase)))
            SetTabOrder(static_cast<int64_t>(json->GetInt64(vcc::ConvertNamingStyle(L"TabOrder", namestyle, vcc::NamingStyle::PascalCase))));
        // GitForms
        ClearGitForms();
        if (json->IsContainKey(vcc::ConvertNamingStyle(L"GitForms", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(vcc::ConvertNamingStyle(L"GitForms", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpGitForms = std::make_shared<VPGGitForm>();
                tmpGitForms->DeserializeJson(element->GetArrayElementObject());
                InsertGitForms(tmpGitForms);
            }
        }
    CATCH
}

void VPGWorkspaceForm::InitializeComponents()
{
    TRY
        vcc::BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IResult> VPGWorkspaceForm::DoAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> /*argument*/)
{
    TRY
        switch(static_cast<VPGWorkspaceFormProperty>(formProperty))
        {
        default:
            assert(false);
            break;
        }
    CATCH
    return nullptr;
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
