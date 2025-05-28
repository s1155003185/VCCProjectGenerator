// <vcc:vccproj sync="FULL" gen="FULL"/>
#include "vpg_main_form.hpp"

#include <assert.h>
#include <memory>
#include <string>

#include "base_action.hpp"
#include "base_form.hpp"
#include "exception_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "i_object.hpp"
#include "i_result.hpp"
#include "json.hpp"
#include "log_config.hpp"
#include "number_helper.hpp"
#include "operation_result.hpp"
#include "string_helper.hpp"
#include "vpg_main_form_property.hpp"
#include "vpg_workspace_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
#include "file_helper.hpp"
#include "i_property_accessor.hpp"
#include "lock_type.hpp"
#include "property_accessor_factory.hpp"
#include "vpg_class_helper.hpp"
#include "vpg_global.hpp"
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
        return L"VPGMainFormInitialize start";
        // </vcc:VPGMainFormInitializeGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormInitialize::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormInitializeGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"VPGMainFormInitialize complete";
        // </vcc:VPGMainFormInitializeGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<IResult> VPGMainFormInitialize::OnRedo()
{
    TRY
        // <vcc:VPGMainFormInitializeOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::Create(_ParentObject);
        propertyAccessor->WriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        form->TruncateAction();
        form->ClearWorkspaceForms();

        auto configFilePath = VPGGlobal::GetVCCProjectManagerConfigFileFullPath();
        if (IsFilePresent(configFilePath)) {
            TRY
                auto jsonBuilder = std::make_unique<JsonBuilder>();
                jsonBuilder->SetIsBeautify(true);
                form->DeserializeJsonString(jsonBuilder.get(), ReadFile(configFilePath));
            CATCH_MSG(ExceptionType::ParserError, L"File " + configFilePath + L" exists but not vaild. Please adjust / remove the file and try again")
        } else {
            auto workspace = std::make_shared<VPGWorkspaceForm>();
            workspace->SetName(L"Default");
            form->InsertWorkspaceForms(workspace);
            form->SaveConfig();
        }
        propertyAccessor->Unlock();
        // </vcc:VPGMainFormInitializeOnRedo>
    CATCH_RETURN_RESULT(OperationResult)
    return std::make_shared<OperationResult>();
}

VPGMainFormAddWorkspaceForm::VPGMainFormAddWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormAddWorkspaceForm::VPGMainFormAddWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm, std::shared_ptr<VPGMainFormAddWorkspaceFormArgument> argument) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormAddWorkspaceForm::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormAddWorkspaceForm start";
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceForm::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormAddWorkspaceForm complete";
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<IResult> VPGMainFormAddWorkspaceForm::OnRedo()
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::Create(_ParentObject);
        propertyAccessor->ReadWriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        auto workspaceForm = std::make_shared<VPGWorkspaceForm>();
        workspaceForm->SetName(_Argument->GetName());
        form->InsertWorkspaceForms(workspaceForm);
        form->SaveConfig();
        propertyAccessor->Unlock();
        // </vcc:VPGMainFormAddWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(OperationResult)
    return std::make_shared<OperationResult>();
}

VPGMainFormDeleteWorkspaceForm::VPGMainFormDeleteWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormDeleteWorkspaceForm::VPGMainFormDeleteWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm, std::shared_ptr<VPGMainFormDeleteWorkspaceFormArgument> argument) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormDeleteWorkspaceForm::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormDeleteWorkspaceForm start";
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspaceForm::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormDeleteWorkspaceForm complete";
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<IResult> VPGMainFormDeleteWorkspaceForm::OnRedo()
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::Create(_ParentObject);
        propertyAccessor->ReadWriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        auto index = form->FindWorkspaceForms(_Argument->GetWorkspaceForm());
        if (index < 0)
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace not found");
        form->RemoveWorkspaceFormsAtIndex(index);
        form->SaveConfig();
        propertyAccessor->Unlock();
        // </vcc:VPGMainFormDeleteWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(OperationResult)
    return std::make_shared<OperationResult>();
}

VPGMainFormRenameWorkspaceForm::VPGMainFormRenameWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormRenameWorkspaceForm::VPGMainFormRenameWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm, std::shared_ptr<VPGMainFormRenameWorkspaceFormArgument> argument) : BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormRenameWorkspaceForm::GetRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormRenameWorkspaceForm start";
        // </vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceForm::GetRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"Execute VPGMainFormRenameWorkspaceForm complete";
        // </vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<IResult> VPGMainFormRenameWorkspaceForm::OnRedo()
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormRenameWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(OperationResult)
    return std::make_shared<OperationResult>();
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

std::shared_ptr<Json> VPGMainForm::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // WorkspaceForms
        auto tmpWorkspaceForms = std::make_shared<Json>();
        json->AddArray(ConvertNamingStyle(L"WorkspaceForms", NamingStyle::PascalCase, namestyle), tmpWorkspaceForms);
        for (auto const &element : GetWorkspaceForms()) {
            tmpWorkspaceForms->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGMainForm::DeserializeJson(std::shared_ptr<IDocument> document)
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);
        // WorkspaceForms
        ClearWorkspaceForms();
        if (json->IsContainKey(ConvertNamingStyle(L"WorkspaceForms", namestyle, NamingStyle::PascalCase))) {
            for (auto const &element : json->GetArray(ConvertNamingStyle(L"WorkspaceForms", namestyle, NamingStyle::PascalCase))) {
                auto tmpWorkspaceForms = std::make_shared<VPGWorkspaceForm>();
                tmpWorkspaceForms->DeserializeJson(element->GetArrayElementObject());
                InsertWorkspaceForms(tmpWorkspaceForms);
            }
        }
    CATCH
}

void VPGMainForm::InitializeComponents()
{
    TRY
        BaseForm::InitializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<IResult> VPGMainForm::DoAction(const int64_t &formProperty, std::shared_ptr<IObject> argument)
{
    TRY
        switch(static_cast<VPGMainFormProperty>(formProperty))
        {
        case VPGMainFormProperty::Initialize:
            return DoInitialize();
        case VPGMainFormProperty::AddWorkspaceForm:
            return DoAddWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(argument));
        case VPGMainFormProperty::DeleteWorkspaceForm:
            return DoDeleteWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(argument));
        case VPGMainFormProperty::RenameWorkspaceForm:
            return DoRenameWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(argument));
        default:
            assert(false);
            break;
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IResult> VPGMainForm::DoInitialize()
{
    TRY
        auto action = std::make_shared<VPGMainFormInitialize>(_LogConfig, SharedPtr());
        // <vcc:VPGMainFormDoInitialize sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoInitialize>
        return ExecuteAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<IResult> VPGMainForm::DoAddWorkspaceForm(std::shared_ptr<VPGMainFormAddWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormAddWorkspaceForm>(_LogConfig, SharedPtr(), argument);
        // <vcc:VPGMainFormDoAddWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoAddWorkspaceForm>
        return ExecuteAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<IResult> VPGMainForm::DoDeleteWorkspaceForm(std::shared_ptr<VPGMainFormDeleteWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormDeleteWorkspaceForm>(_LogConfig, SharedPtr(), argument);
        // <vcc:VPGMainFormDoDeleteWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoDeleteWorkspaceForm>
        return ExecuteAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<IResult> VPGMainForm::DoRenameWorkspaceForm(std::shared_ptr<VPGMainFormRenameWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormRenameWorkspaceForm>(_LogConfig, SharedPtr(), argument);
        // <vcc:VPGMainFormDoRenameWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoRenameWorkspaceForm>
        return ExecuteAction(action, true);
    CATCH
    return nullptr;
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
void VPGMainForm::SaveConfig() const
{
    TRY    
        auto jsonBuilder = std::make_unique<JsonBuilder>();
        jsonBuilder->SetIsBeautify(true);
        WriteFile(VPGGlobal::GetVCCProjectManagerConfigFileFullPath(), SerializeJson(jsonBuilder.get()), true);
    CATCH
}
// </vcc:customFunctions>
