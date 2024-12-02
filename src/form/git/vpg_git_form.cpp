// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_git_form.hpp"

#include <assert.h>
#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "log_config.hpp"
#include "vpg_git_form_property.hpp"

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
    std::shared_ptr<VPGGitForm> obj = std::make_shared<VPGGitForm>(*this);
    obj->CloneLog(this->_Log);
    return obj;
}

void VPGGitForm::Initialize() const
{
    TRY
        BaseForm::Initialize();
        _LogConfig = std::make_shared<LogConfig>();
        _ActionManager = std::make_shared<ActionManager>(_LogConfig);
        OnInitialize();
    CATCH
}

void VPGGitForm::DoAction(const int64_t &formProperty) const
{
    TRY
        switch(static_cast<VPGGitFormProperty>(formProperty))
        {
        default:
            assert(false);
            break;
        }
    CATCH
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
