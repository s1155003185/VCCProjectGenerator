// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_workspace_form.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "base_form.hpp"
#include "exception_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "number_helper.hpp"
#include "string_helper.hpp"
#include "vpg_git_form.hpp"
#include "vpg_workspace_form_property.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

VPGWorkspaceForm::VPGWorkspaceForm() : BaseForm()
{
    TRY
        _ObjectType = ObjectType::WorkspaceForm;
        Initialize();
    CATCH
}

std::shared_ptr<IObject> VPGWorkspaceForm::Clone() const
{
    auto obj = std::make_shared<VPGWorkspaceForm>(*this);
    obj->CloneGitForms(this->_GitForms);
    return obj;
}

std::shared_ptr<Json> VPGWorkspaceForm::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // Name
        json->AddString(ConvertNamingStyle(L"Name", NamingStyle::PascalCase, namestyle), _Name);
        // GitForms
        auto tmpGitForms = std::make_shared<Json>();
        json->AddArray(ConvertNamingStyle(L"GitForms", NamingStyle::PascalCase, namestyle), tmpGitForms);
        for (auto const &element : _GitForms) {
            tmpGitForms->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGWorkspaceForm::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // Name
        if (json->IsContainKey(ConvertNamingStyle(L"Name", namestyle, NamingStyle::PascalCase)))
            _Name = json->GetString(ConvertNamingStyle(L"Name", namestyle, NamingStyle::PascalCase));
        // GitForms
        ClearGitForms();
        if (json->IsContainKey(ConvertNamingStyle(L"GitForms", namestyle, NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(ConvertNamingStyle(L"GitForms", namestyle, NamingStyle::PascalCase))) {
                auto tmpGitForms = std::make_shared<VPGGitForm>();
                tmpGitForms->DeserializeJson(element->GetArrayElementObject());
                InsertGitForms(tmpGitForms);
            }
        }
    CATCH
}

void VPGWorkspaceForm::InitializeComponents() const
{
    TRY
        BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        OnInitializeComponents();
    CATCH
}

void VPGWorkspaceForm::DoAction(const int64_t &formProperty, std::shared_ptr<IObject> /*argument*/)
{
    TRY
        switch(static_cast<VPGWorkspaceFormProperty>(formProperty))
        {
        default:
            assert(false);
            break;
        }
    CATCH
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
