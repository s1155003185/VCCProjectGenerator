#include "application.hpp"

#include <assert.h>
#include <map>

#include "base_form.hpp"
#include "exception_macro.hpp"
#include "object_factory.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

void Application::InitializeComponents() const
{
    TRY
        // Initialize for Global Log and Thread Setting
        // Default No Action
        _LogConfig = std::make_shared<LogConfig>();
        _ActionManager = nullptr;
        _ThreadManager = std::make_shared<ThreadManager>(_LogConfig);

        // Custom Initialize Log, Action, Thread by override OnInitializeComponents
        OnInitializeComponents();
    CATCH
}

std::shared_ptr<IObject> Application::GetFormSharedPtr(IObject *form)
{
    TRY
        for (auto tmpForm : application->_Forms) {
            if (tmpForm.get() == form)
                return tmpForm;
        }
    CATCH
    return nullptr;
}

const IForm *Application::GetIFormPtrFromIObject(IObject *obj)
{
    TRY
        return static_cast<BaseForm *>(obj);
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

std::shared_ptr<IObject> Application::CreateForm(const ObjectType &objectType)
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

void Application::DoFormAction(IObject *form, const int64_t &formProperty)
{
    TRY
        if (form == nullptr)
            return;
        GetIFormPtrFromIObject(form)->DoAction(formProperty);
    CATCH
}

int64_t Application::GetFormActionFirstSeqNo(IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->GetActionFirstSeqNo();
    CATCH
    return -1;
}

int64_t Application::GetFormActionLastSeqNo(IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->GetActionLastSeqNo();
    CATCH
    return -1;
}

int64_t Application::RedoFormAction(IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->RedoAction(noOfStep);
    CATCH
    return -1;
}

int64_t Application::RedoFormActionToSeqNo(IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->RedoActionToSeqNo(seqNo);
    CATCH
    return -1;
}

int64_t Application::UndoFormAction(IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->UndoAction(noOfStep);
    CATCH
    return -1;
}

int64_t Application::UndoFormActionToSeqNo(IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->UndoActionToSeqNo(seqNo);
    CATCH
    return -1;
}

int64_t Application::ClearFormAction(IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->ClearAction();
    CATCH
    return -1;
}

int64_t Application::TruncateFormAction(IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return GetIFormPtrFromIObject(form)->TruncateAction();
    CATCH
    return -1;
}

bool Application::IsFormClosable(IObject *form)
{
    if (form == nullptr)
        return true;
    
    TRY
        return GetIFormPtrFromIObject(form)->IsClosable();
    CATCH
    return false;
}

bool Application::IsFormClosed(IObject *form)
{
    TRY
        if (form == nullptr)
            return true;
        return GetIFormPtrFromIObject(form)->IsClosed();
    CATCH
    return true;
}

bool Application::CloseForm(IObject *form, const bool &isForce)
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