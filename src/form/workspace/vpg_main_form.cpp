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

VPGMainFormInitialize::VPGMainFormInitialize(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormInitialize::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormInitializeGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormInitializeGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormInitialize::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormInitializeGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormInitializeGetRedoMessageComplete>
    CATCH
    return L"";
}

void VPGMainFormInitialize::OnRedo()
{
    TRY
        // <vcc:VPGMainFormInitializeOnRedo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormInitializeOnRedo>
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

void VPGMainForm::DoAction(const int64_t &formProperty)
{
    TRY
        switch(static_cast<VPGMainFormProperty>(formProperty))
        {
        case VPGMainFormProperty::Initialize:
            DoInitialize();
            break;
        default:
            assert(false);
            break;
        }
    CATCH
}

void VPGMainForm::DoInitialize()
{
    TRY
        auto action = std::make_shared<VPGMainFormInitialize>(_LogConfig, SharedPtr());
        // <vcc:VPGMainFormDoInitialize sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoInitialize>
        ExecuteAction(action, true);
    CATCH
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customFunctions>
