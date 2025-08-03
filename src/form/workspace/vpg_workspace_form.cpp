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
        initialize();
    CATCH
}

std::shared_ptr<vcc::IObject> VPGWorkspaceForm::clone() const
{
    auto obj = std::make_shared<VPGWorkspaceForm>(*this);
    obj->cloneGitForms(this->_GitForms);
    return obj;
}

std::shared_ptr<vcc::Json> VPGWorkspaceForm::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // Name
        json->addString(vcc::convertNamingStyle(L"Name", vcc::NamingStyle::PascalCase, namestyle), getName());
        // TabOrder
        json->addInt(vcc::convertNamingStyle(L"TabOrder", vcc::NamingStyle::PascalCase, namestyle), getTabOrder());
        // GitForms
        auto tmpGitForms = std::make_shared<vcc::Json>();
        json->addArray(vcc::convertNamingStyle(L"GitForms", vcc::NamingStyle::PascalCase, namestyle), tmpGitForms);
        for (auto const &element : getGitForms()) {
            tmpGitForms->addArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGWorkspaceForm::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // Name
        if (json->isContainKey(vcc::convertNamingStyle(L"Name", namestyle, vcc::NamingStyle::PascalCase)))
            setName(json->getString(vcc::convertNamingStyle(L"Name", namestyle, vcc::NamingStyle::PascalCase)));
        // TabOrder
        if (json->isContainKey(vcc::convertNamingStyle(L"TabOrder", namestyle, vcc::NamingStyle::PascalCase)))
            setTabOrder(static_cast<int64_t>(json->getInt64(vcc::convertNamingStyle(L"TabOrder", namestyle, vcc::NamingStyle::PascalCase))));
        // GitForms
        clearGitForms();
        if (json->isContainKey(vcc::convertNamingStyle(L"GitForms", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::convertNamingStyle(L"GitForms", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpGitForms = std::make_shared<VPGGitForm>();
                tmpGitForms->deserializeJson(element->getArrayElementObject());
                insertGitForms(tmpGitForms);
            }
        }
    CATCH
}

void VPGWorkspaceForm::initializeComponents()
{
    TRY
        vcc::BaseForm::initializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        onInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IResult> VPGWorkspaceForm::doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> /*argument*/)
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
