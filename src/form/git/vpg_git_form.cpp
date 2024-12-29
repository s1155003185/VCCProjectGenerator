// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_git_form.hpp"

#include <assert.h>
#include <memory>

#include "base_form.hpp"
#include "exception_macro.hpp"
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
    auto obj = std::make_shared<VPGGitForm>(*this);
    obj->CloneLog(this->_Log);
    return obj;
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

void VPGGitForm::DoAction(const int64_t &formProperty)
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
