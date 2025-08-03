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

VPGMainFormAddWorkspaceForm::VPGMainFormAddWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormAddWorkspaceForm::VPGMainFormAddWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormAddWorkspaceFormArgument> argument) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormAddWorkspaceForm::getRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormAddWorkspaceForm start";
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceForm::getRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormAddWorkspaceForm complete";
        // </vcc:VPGMainFormAddWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<vcc::IResult> VPGMainFormAddWorkspaceForm::onRedo()
{
    TRY
        // <vcc:VPGMainFormAddWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::create(_ParentObject);
        propertyAccessor->readWriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        auto workspaceForm = std::make_shared<VPGWorkspaceForm>();
        workspaceForm->setName(_Argument->getName());
        form->insertWorkspaceForms(workspaceForm);
        form->saveConfig();
        propertyAccessor->unlock();
        // </vcc:VPGMainFormAddWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(vcc::OperationResult)
    return std::make_shared<vcc::OperationResult>();
}

VPGMainFormDeleteWorkspaceForm::VPGMainFormDeleteWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormDeleteWorkspaceForm::VPGMainFormDeleteWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormDeleteWorkspaceFormArgument> argument) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormDeleteWorkspaceForm::getRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormDeleteWorkspaceForm start";
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormDeleteWorkspaceForm::getRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormDeleteWorkspaceForm complete";
        // </vcc:VPGMainFormDeleteWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<vcc::IResult> VPGMainFormDeleteWorkspaceForm::onRedo()
{
    TRY
        // <vcc:VPGMainFormDeleteWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::create(_ParentObject);
        propertyAccessor->readWriteLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        auto index = form->findWorkspaceForms(_Argument->getWorkspaceForm());
        if (index < 0)
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Workspace not found");
        form->removeWorkspaceFormsAtIndex(index);
        form->saveConfig();
        propertyAccessor->unlock();
        // </vcc:VPGMainFormDeleteWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(vcc::OperationResult)
    return std::make_shared<vcc::OperationResult>();
}

VPGMainFormInitialize::VPGMainFormInitialize(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

std::wstring VPGMainFormInitialize::getRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormInitializeGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"VPGMainFormInitialize start";
        // </vcc:VPGMainFormInitializeGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormInitialize::getRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormInitializeGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"VPGMainFormInitialize complete";
        // </vcc:VPGMainFormInitializeGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<vcc::IResult> VPGMainFormInitialize::onRedo()
{
    TRY
        // <vcc:VPGMainFormInitializeOnRedo sync="RESERVE" gen="RESERVE">
        auto propertyAccessor = PropertyAccessorFactory::create(_ParentObject);
        propertyAccessor->writeLock();
        auto form = std::dynamic_pointer_cast<VPGMainForm>(_ParentObject);
        form->truncateAction();
        form->clearWorkspaceForms();

        auto configFilePath = VPGGlobal::getVCCProjectManagerConfigFileFullPath();
        if (vcc::isFilePresent(configFilePath)) {
            TRY
                auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
                jsonBuilder->setIsBeautify(true);
                form->deserializeJsonString(jsonBuilder.get(), vcc::readFile(configFilePath));
            CATCH_MSG(ExceptionType::ParserError, L"File " + configFilePath + L" exists but not vaild. Please adjust / remove the file and try again")
        } else {
            auto workspace = std::make_shared<VPGWorkspaceForm>();
            workspace->setName(L"Default");
            form->insertWorkspaceForms(workspace);
            form->saveConfig();
        }
        propertyAccessor->unlock();
        // </vcc:VPGMainFormInitializeOnRedo>
    CATCH_RETURN_RESULT(vcc::OperationResult)
    return std::make_shared<vcc::OperationResult>();
}

VPGMainFormRenameWorkspaceForm::VPGMainFormRenameWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
}

VPGMainFormRenameWorkspaceForm::VPGMainFormRenameWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormRenameWorkspaceFormArgument> argument) : vcc::BaseAction()
{
    _LogConfig = logConfig;
    _ParentObject = parentForm;
    _Argument = argument;
}

std::wstring VPGMainFormRenameWorkspaceForm::getRedoMessageStart() const
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageStart sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormRenameWorkspaceForm start";
        // </vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageStart>
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceForm::getRedoMessageComplete() const
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageComplete sync="RESERVE" gen="RESERVE">
        return L"execute VPGMainFormRenameWorkspaceForm complete";
        // </vcc:VPGMainFormRenameWorkspaceFormGetRedoMessageComplete>
    CATCH
    return L"";
}

std::shared_ptr<vcc::IResult> VPGMainFormRenameWorkspaceForm::onRedo()
{
    TRY
        // <vcc:VPGMainFormRenameWorkspaceFormOnRedo sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormRenameWorkspaceFormOnRedo>
    CATCH_RETURN_RESULT(vcc::OperationResult)
    return std::make_shared<vcc::OperationResult>();
}

VPGMainForm::VPGMainForm() : vcc::BaseForm()
{
    TRY
        _ObjectType = ObjectType::MainForm;
        initialize();
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainForm::clone() const
{
    auto obj = std::make_shared<VPGMainForm>(*this);
    obj->cloneWorkspaceForms(this->_WorkspaceForms);
    return obj;
}

std::shared_ptr<vcc::Json> VPGMainForm::ToJson() const
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::make_unique<vcc::Json>();
        // WorkspaceForms
        auto tmpWorkspaceForms = std::make_shared<vcc::Json>();
        json->addArray(vcc::convertNamingStyle(L"WorkspaceForms", vcc::NamingStyle::PascalCase, namestyle), tmpWorkspaceForms);
        for (auto const &element : getWorkspaceForms()) {
            tmpWorkspaceForms->addArrayObject(element->ToJson());
        }
        return json;
    CATCH
    return nullptr;
}

void VPGMainForm::deserializeJson(std::shared_ptr<vcc::IDocument> document)
{
    TRY
        vcc::NamingStyle namestyle = vcc::NamingStyle::PascalCase;
        auto json = std::dynamic_pointer_cast<vcc::Json>(document);
        assert(json != nullptr);
        // WorkspaceForms
        clearWorkspaceForms();
        if (json->isContainKey(vcc::convertNamingStyle(L"WorkspaceForms", namestyle, vcc::NamingStyle::PascalCase))) {
            for (auto const &element : json->getArray(vcc::convertNamingStyle(L"WorkspaceForms", namestyle, vcc::NamingStyle::PascalCase))) {
                auto tmpWorkspaceForms = std::make_shared<VPGWorkspaceForm>();
                tmpWorkspaceForms->deserializeJson(element->getArrayElementObject());
                insertWorkspaceForms(tmpWorkspaceForms);
            }
        }
    CATCH
}

void VPGMainForm::initializeComponents()
{
    TRY
        vcc::BaseForm::initializeComponents();
        _LogConfig = nullptr;
        _ActionManager = nullptr;
        _ThreadManager = nullptr;
        onInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IResult> VPGMainForm::doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> argument)
{
    TRY
        switch(static_cast<VPGMainFormProperty>(formProperty))
        {
        case VPGMainFormProperty::Initialize:
            return doInitialize();
        case VPGMainFormProperty::AddWorkspaceForm:
            return doAddWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(argument));
        case VPGMainFormProperty::DeleteWorkspaceForm:
            return doDeleteWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(argument));
        case VPGMainFormProperty::RenameWorkspaceForm:
            return doRenameWorkspaceForm(std::dynamic_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(argument));
        default:
            assert(false);
            break;
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IResult> VPGMainForm::doInitialize()
{
    TRY
        auto action = std::make_shared<VPGMainFormInitialize>(_LogConfig, sharedPtr());
        // <vcc:VPGMainFormDoInitialize sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoInitialize>
        return executeAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IResult> VPGMainForm::doAddWorkspaceForm(std::shared_ptr<VPGMainFormAddWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormAddWorkspaceForm>(_LogConfig, sharedPtr(), argument);
        // <vcc:VPGMainFormDoAddWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoAddWorkspaceForm>
        return executeAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IResult> VPGMainForm::doDeleteWorkspaceForm(std::shared_ptr<VPGMainFormDeleteWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormDeleteWorkspaceForm>(_LogConfig, sharedPtr(), argument);
        // <vcc:VPGMainFormDoDeleteWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoDeleteWorkspaceForm>
        return executeAction(action, true);
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IResult> VPGMainForm::doRenameWorkspaceForm(std::shared_ptr<VPGMainFormRenameWorkspaceFormArgument> argument)
{
    TRY
        auto action = std::make_shared<VPGMainFormRenameWorkspaceForm>(_LogConfig, sharedPtr(), argument);
        // <vcc:VPGMainFormDoRenameWorkspaceForm sync="RESERVE" gen="RESERVE">
        // </vcc:VPGMainFormDoRenameWorkspaceForm>
        return executeAction(action, true);
    CATCH
    return nullptr;
}

// <vcc:customFunctions sync="RESERVE" gen="RESERVE">
void VPGMainForm::saveConfig() const
{
    TRY    
        auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
        jsonBuilder->setIsBeautify(true);
        vcc::writeFile(VPGGlobal::getVCCProjectManagerConfigFileFullPath(), serializeJson(jsonBuilder.get()), true);
    CATCH
}
// </vcc:customFunctions>
