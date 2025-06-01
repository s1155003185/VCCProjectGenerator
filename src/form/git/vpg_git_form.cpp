// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_git_form.hpp"

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
#include "vpg_git_form_property.hpp"
#include "vpg_git_log.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

VPGGitForm::VPGGitForm() : vcc::BaseForm()
{
    TRY
        _ObjectType = ObjectType::GitForm;
        Initialize();
    CATCH
}

std::shared_ptr<vcc::IObject> VPGGitForm::Clone() const
{
    auto obj = std::make_shared<VPGGitForm>(*this);
    obj->CloneLog(this->_Log.get());
    return obj;
}

std::shared_ptr<vcc::Json> VPGGitForm::ToJson() const
{
    TRY
        auto json = std::make_unique<vcc::Json>();
        return json;
    CATCH
    return nullptr;
}

void VPGGitForm::DeserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
    CATCH
}

void VPGGitForm::InitializeComponents()
{
    TRY
        vcc::BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        // Custom Managers
        _GitManager = std::make_shared<vcc::GitManager>(_LogConfig);
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IResult> VPGGitForm::DoAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> /*argument*/)
{
    TRY
        switch(static_cast<VPGGitFormProperty>(formProperty))
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
