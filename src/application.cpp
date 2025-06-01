#include "application.hpp"

#include <assert.h>
#include <map>
#include <memory>
#include <set>

#include "base_form.hpp"
#include "base_result.hpp"
#include "exception_macro.hpp"
#include "i_object.hpp"
#include "object_factory.hpp"
#include "object_type.hpp"
#include "set_helper.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

void Application::InitializeComponents()
{
    TRY
        // Initialize for Global Log and Thread Setting
        _LogConfig = std::make_shared<vcc::LogConfig>();
        _ThreadManager = std::make_shared<vcc::ThreadManager>(_LogConfig);

        // <vcc:vccconfig sync="RESERVE" gen="REPLACE">
        _ActionManager = nullptr;
        // </vcc:vccconfig>
        // Custom Initialize Log, Action, Thread by override OnInitializeComponents
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IObject> Application::GetFormSharedPtr(vcc::IObject *form)
{
    TRY
        for (auto tmpForm : application->_Forms) {
            if (tmpForm.get() == form)
                return tmpForm;
        }
    CATCH
    return nullptr;
}

vcc::IForm *Application::GetIFormPtrFromIObject(vcc::IObject *obj)
{
    TRY
        return static_cast<BaseForm *>(obj);
    CATCH
    return nullptr;
}

vcc::IObject *Application::GetIObjectPtrFromIResult(vcc::IResult *obj)
{
    TRY
        return static_cast<vcc::BaseResult *>(obj);
    CATCH
    return nullptr;
}

vcc::IResult *Application::GetIResultPtrFromIObject(vcc::IObject *obj)
{
    TRY
        return static_cast<vcc::BaseResult *>(obj);
    CATCH
    return nullptr;
}

void Application::Run()
{
    TRY
        if (application == nullptr)
            application = std::make_unique<Application>();
    CATCH
}

std::shared_ptr<vcc::IObject> Application::CreateForm(const ObjectType &objectType)
{
    TRY
        auto form = ObjectFactory::Create(objectType);
        assert(form != nullptr);
        form->SetParentObject(application);
        application->_Forms.insert(form);
        return form;
    CATCH
    return nullptr;
}

int64_t Application::GetResultErrorCode(vcc::IObject *result)
{
    TRY
        return static_cast<int64_t>(GetIResultPtrFromIObject(result)->GetExceptionType());
    CATCH
    return -1;
}

std::wstring Application::GetResultMessage(vcc::IObject *result)
{
    TRY
        return GetIResultPtrFromIObject(result)->GetMessage();
    CATCH
    return L"";
}

bool Application::IsErrorResult(vcc::IObject *result)
{
    TRY
        return GetIResultPtrFromIObject(result)->IsError();
    CATCH
    return false;
}

bool Application::IsWarningResult(vcc::IObject *result)
{
    TRY
        return GetIResultPtrFromIObject(result)->IsWarning();
    CATCH
    return false;
}

void Application::EraseResult(vcc::IObject *result)
{
    TRY
        RemoveIObjectAll(application->_Results, result);
    CATCH
}

std::shared_ptr<vcc::IObject> Application::CreateActionArgument(const ObjectType &objectType)
{
    TRY
        auto actionArgument = ObjectFactory::Create(objectType);
        application->_ActionArguments.insert(actionArgument);
        return actionArgument;
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> Application::DoFormAction(vcc::IObject *form, const int64_t &formProperty, vcc::IObject *argument)
{
    TRY
        if (form == nullptr)
            return nullptr;
        auto result = GetIFormPtrFromIObject(form)->DoAction(formProperty, argument != nullptr ? argument->SharedPtr() : nullptr);
        if (argument != nullptr)
            RemoveIObjectAll(application->_ActionArguments, argument);
        auto obj = GetIObjectPtrFromIResult(result.get())->SharedPtr();
        application->_Results.insert(obj);
        return obj;
    CATCH
    return nullptr;
}

int64_t Application::GetFormActionCurrentSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->GetActionCurrentSeqNo();
    CATCH
    return -1;
}

int64_t Application::GetFormActionFirstSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->GetActionFirstSeqNo();
    CATCH
    return -1;
}

int64_t Application::GetFormActionLastSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->GetActionLastSeqNo();
    CATCH
    return -1;
}

void Application::RedoFormAction(vcc::IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return;
        GetIObjectPtrFromIResult(GetIFormPtrFromIObject(form)->RedoAction(noOfStep).get())->SharedPtr();
    CATCH
}

void Application::RedoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return;
        GetIObjectPtrFromIResult(GetIFormPtrFromIObject(form)->RedoActionToSeqNo(seqNo).get())->SharedPtr();
    CATCH
}

void Application::UndoFormAction(vcc::IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return;
        GetIObjectPtrFromIResult(GetIFormPtrFromIObject(form)->UndoAction(noOfStep).get())->SharedPtr();
    CATCH
}

void Application::UndoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return;
        GetIObjectPtrFromIResult(GetIFormPtrFromIObject(form)->UndoActionToSeqNo(seqNo).get())->SharedPtr();
    CATCH
}

int64_t Application::ClearFormAction(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->ClearAction();
    CATCH
    return -1;
}

int64_t Application::TruncateFormAction(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->TruncateAction();
    CATCH
    return -1;
}

bool Application::IsFormClosable(vcc::IObject *form)
{
    if (form == nullptr)
        return true;
    
    TRY
        return GetIFormPtrFromIObject(form)->IsClosable();
    CATCH
    return false;
}

bool Application::IsFormClosed(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return true;
        return GetIFormPtrFromIObject(form)->IsClosed();
    CATCH
    return true;
}

bool Application::CloseForm(vcc::IObject *form, const bool &isForce)
{
    if (form == nullptr)
        return true;
    
    TRY
        if (GetIFormPtrFromIObject(form)->Close(isForce)) {
            if (GetFormSharedPtr(form) != nullptr)
                application->_Forms.erase(GetFormSharedPtr(form));
            return true;
        }
    CATCH
    return false;
}

// <vcc:customApplicationFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customApplicationFunctions>