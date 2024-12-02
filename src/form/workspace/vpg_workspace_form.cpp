// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_workspace_form.hpp"

#include <assert.h>
#include <memory>

#include "action_manager.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "log_config.hpp"
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
    std::shared_ptr<VPGWorkspaceForm> obj = std::make_shared<VPGWorkspaceForm>(*this);
    obj->CloneGitForms(this->_GitForms);
    return obj;
}

void VPGWorkspaceForm::Initialize() const
{
    TRY
        BaseForm::Initialize();
        _LogConfig = std::make_shared<LogConfig>();
        _ActionManager = std::make_shared<ActionManager>(_LogConfig);
        OnInitialize();
    CATCH
}

void VPGWorkspaceForm::DoAction(const int64_t &formProperty) const
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
