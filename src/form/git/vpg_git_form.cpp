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

using namespace vcc;

VPGGitForm::VPGGitForm() : BaseForm()
{
    TRY
        _ObjectType = ObjectType::GitForm;
        Initialize();
    CATCH
}

std::shared_ptr<IObject> VPGGitForm::Clone() const
{
    auto obj = std::make_shared<VPGGitForm>(*this);
    obj->CloneLog(this->_Log.get());
    return obj;
}

std::shared_ptr<Json> VPGGitForm::ToJson() const
{
    TRY
        auto json = std::make_unique<Json>();
        return json;
    CATCH
    return nullptr;
}

void VPGGitForm::DeserializeJson(std::shared_ptr<IDocument> document) const
{
    TRY
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
    CATCH
}

void VPGGitForm::InitializeComponents() const
{
    TRY
        BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        // Custom Managers
        _GitManager = std::make_shared<GitManager>(_LogConfig);
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<IResult> VPGGitForm::DoAction(const int64_t &formProperty, std::shared_ptr<IObject> /*argument*/)
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
