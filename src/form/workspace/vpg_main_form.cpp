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
#include "json.hpp"
#include "log_config.hpp"
#include "number_helper.hpp"
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

void VPGMainFormInitialize::OnRedo()
{
    TRY
        // <vcc:VPGMainFormInitializeOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::Create(_ParentObject);
        propertyAccessor->WriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        form->TruncateAction();
        form->ClearWorkspaceForms();

        auto jsonBuilder = std::make_unique<JsonBuilder>();
        jsonBuilder->SetIsBeautify(true);

        auto configFilePath = VPGGlobal::GetVCCProjectManagerConfigFileFullPath();
        if (IsFilePresent(configFilePath)) {
            TRY
                form->DeserializeJsonString(jsonBuilder.get(), ReadFile(configFilePath));
            CATCH_MSG(ExceptionType::ParserError, L"File " + configFilePath + L" exists but not vaild. Please adjust / remove the file and try again")
        } else {
            auto workspace = std::make_shared<VPGWorkspaceForm>();
            workspace->SetName(L"Default");
            form->InsertWorkspaceForms(workspace);
            WriteFile(configFilePath, form->SerializeJson(jsonBuilder.get()), true);
        }
        propertyAccessor->Unlock();
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

std::shared_ptr<Json> VPGMainForm::ToJson() const
{
    TRY
        NamingStyle namestyle = NamingStyle::PascalCase;
        auto json = std::make_unique<Json>();
        // WorkspaceForms
        auto tmpWorkspaceForms = std::make_shared<Json>();
        json->AddArray(ConvertNamingStyle(L"WorkspaceForms", NamingStyle::PascalCase, namestyle), tmpWorkspaceForms);
        for (auto const &element : _WorkspaceForms) {
            tmpWorkspaceForms->AddArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGMainForm::DeserializeJson(std::shared_ptr<IDocument> document) const
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
