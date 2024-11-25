// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_git_form.hpp"

#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "log_config.hpp"

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

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
