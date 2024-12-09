// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_workspace_form.hpp"

#include <assert.h>
#include <memory>

#include "base_form.hpp"
#include "exception_macro.hpp"
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
