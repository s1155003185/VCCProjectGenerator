// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_main_form.hpp"

#include <assert.h>
#include <memory>

#include "base_form.hpp"
#include "exception_macro.hpp"
#include "vpg_main_form_property.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

VPGMainForm::VPGMainForm() : BaseForm()
{
    TRY
        _ObjectType = ObjectType::MainForm;
        Initialize();
    CATCH
}

std::shared_ptr<IObject> VPGMainForm::Clone() const
{
    auto obj = std::make_shared<VPGMainForm>(*this);
    obj->CloneWorkspaceForms(this->_WorkspaceForms);
    return obj;
}

void VPGMainForm::InitializeComponents() const
{
    TRY
        BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        OnInitializeComponents();
    CATCH
}

void VPGMainForm::DoAction(const int64_t &formProperty) const
{
    TRY
        switch(static_cast<VPGMainFormProperty>(formProperty))
        {
        case VPGMainFormProperty::AddWorkspace:
            DoAddWorkspace();
            break;
        case VPGMainFormProperty::DeleteWorkspace:
            DoDeleteWorkspace();
            break;
        default:
            assert(false);
            break;
        }
    CATCH
}

void VPGMainForm::DoAddWorkspace() const
{
    TRY
    CATCH
}

void VPGMainForm::DoDeleteWorkspace() const
{
    TRY
    CATCH
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
