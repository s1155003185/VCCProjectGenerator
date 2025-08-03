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
        initialize();
    CATCH
}

std::shared_ptr<vcc::IObject> VPGGitForm::clone() const
{
    auto obj = std::make_shared<VPGGitForm>(*this);
    obj->cloneLog(this->_Log.get());
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

void VPGGitForm::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
    CATCH
}

void VPGGitForm::initializeComponents()
{
    TRY
        vcc::BaseForm::initializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        // Custom Managers
        _GitManager = std::make_shared<vcc::GitManager>(_LogConfig);
        onInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IResult> VPGGitForm::doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> /*argument*/)
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
