// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_main_form.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "base_action.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "i_object.hpp"
#include "log_config.hpp"
#include "vpg_main_form_property.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

VPGMainFormAddWorkspace::VPGMainFormAddWorkspace(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormAddWorkspace::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspace::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceGetRedoMessageComplete>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspace::GetUndoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceGetUndoMessageStart sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceGetUndoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspace::GetUndoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceGetUndoMessageComplete sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceGetUndoMessageComplete>
    CATCH
    return L"";
}

void VPGMainFormAddWorkspace::OnRedo() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceOnRedo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceOnRedo>
    CATCH
}

void VPGMainFormAddWorkspace::OnUndo() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceOnUndo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormAddWorkspaceOnUndo>
    CATCH
}

VPGMainFormDeleteWorkspace::VPGMainFormDeleteWorkspace(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormDeleteWorkspace::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspace::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceGetRedoMessageComplete>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspace::GetUndoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceGetUndoMessageStart sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceGetUndoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspace::GetUndoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceGetUndoMessageComplete sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceGetUndoMessageComplete>
    CATCH
    return L"";
}

void VPGMainFormDeleteWorkspace::OnRedo() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceOnRedo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceOnRedo>
    CATCH
}

void VPGMainFormDeleteWorkspace::OnUndo() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceOnUndo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDeleteWorkspaceOnUndo>
    CATCH
}

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
