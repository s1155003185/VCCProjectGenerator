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
#include "i_property_accessor.hpp"
#include "lock_type.hpp"
#include "property_accessor_factory.hpp"
#include "vpg_class_helper.hpp"
// </vcc:customHeader>

using namespace vcc;

VPGMainFormAddWorkspaceForm::VPGMainFormAddWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormAddWorkspaceForm::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return GetActionMessage(L"AddWorkspaceForm", L"Start");
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceForm::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return GetActionMessage(L"AddWorkspaceForm", L"Complete");
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

void VPGMainFormAddWorkspaceForm::OnRedo()
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        PropertyAccessorFactory::Create(_ParentObject)->InsertObjectAtIndex(LockType::WriteLock, static_cast<int64_t>(VPGMainFormProperty::WorkspaceForms), std::make_shared<VPGWorkspaceForm>());
        // </vcc:VPGMainFormAddWorkspaceFormOnRedo>
    CATCH
}

VPGMainFormDeleteWorkspaceForm::VPGMainFormDeleteWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormDeleteWorkspaceForm::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return GetActionMessage(L"DeleteWorkspaceForm", L"Start");
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspaceForm::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return GetActionMessage(L"DeleteWorkspaceForm", L"Complete");
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

void VPGMainFormDeleteWorkspaceForm::OnRedo()
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::Create(_ParentObject);
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        auto obj = form->GetWorkspaceForms().at(0).get();
        propertyAccessor->RemoveObject(LockType::WriteLock, static_cast<int64_t>(VPGMainFormProperty::WorkspaceForms), obj);
        // </vcc:VPGMainFormDeleteWorkspaceFormOnRedo>
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
    obj->CloneCurrentWorkspaceForm(this->_CurrentWorkspaceForm.get());
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

void VPGMainForm::DoAction(const int64_t &formProperty)
{
    TRY
        switch(static_cast<VPGMainFormProperty>(formProperty))
        {
        case VPGMainFormProperty::AddWorkspaceForm:
            DoAddWorkspaceForm();
            break;
        case VPGMainFormProperty::DeleteWorkspaceForm:
            DoDeleteWorkspaceForm();
            break;
        default:
            assert(false);
            break;
        }
    CATCH
}

void VPGMainForm::DoAddWorkspaceForm()
{
    TRY
        auto action = std::make_shared<VPGMainFormAddWorkspaceForm>(_LogConfig, SharedPtr());
        // <vcc:VPGMainFormDoAddWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoAddWorkspaceForm>
        ExecuteAction(action, true);
    CATCH
}

void VPGMainForm::DoDeleteWorkspaceForm()
{
    TRY
        auto action = std::make_shared<VPGMainFormDeleteWorkspaceForm>(_LogConfig, SharedPtr());
        // <vcc:VPGMainFormDoDeleteWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoDeleteWorkspaceForm>
        ExecuteAction(action, true);
    CATCH
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
