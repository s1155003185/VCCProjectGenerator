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

void Application::initializeComponents()
{
    TRY
        // Initialize for Global Log and Thread Setting
        _LogConfig = std::make_shared<vcc::LogConfig>();
        _ThreadManager = std::make_shared<vcc::ThreadManager>(_LogConfig);

        // <vcc:vccconfig sync="RESERVE" gen="REPLACE">
        _ActionManager = nullptr;
        // </vcc:vccconfig>
        // Custom Initialize Log, Action, Thread by override onInitializeComponents
        onInitializeComponents();
    CATCH
}

std::shared_ptr<vcc::IObject> Application::getFormSharedPtr(vcc::IObject *form)
{
    TRY
        for (auto tmpForm : application->_Forms) {
            if (tmpForm.get() == form)
                return tmpForm;
        }
    CATCH
    return nullptr;
}

vcc::IForm *Application::getIFormPtrFromIObject(vcc::IObject *obj)
{
    TRY
        return static_cast<BaseForm *>(obj);
    CATCH
    return nullptr;
}

vcc::IObject *Application::getIObjectPtrFromIResult(vcc::IResult *obj)
{
    TRY
        return static_cast<vcc::BaseResult *>(obj);
    CATCH
    return nullptr;
}

vcc::IResult *Application::getIResultPtrFromIObject(vcc::IObject *obj)
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

std::shared_ptr<vcc::IObject> Application::createForm(const ObjectType &objectType)
{
    TRY
        auto form = ObjectFactory::create(objectType);
        assert(form != nullptr);
        form->setParentObject(application);
        application->_Forms.insert(form);
        return form;
    CATCH
    return nullptr;
}

int64_t Application::getResultErrorCode(vcc::IObject *result)
{
    TRY
        return static_cast<int64_t>(getIResultPtrFromIObject(result)->getExceptionType());
    CATCH
    return -1;
}

std::wstring Application::getResultMessage(vcc::IObject *result)
{
    TRY
        return getIResultPtrFromIObject(result)->getMessage();
    CATCH
    return L"";
}

bool Application::isErrorResult(vcc::IObject *result)
{
    TRY
        return getIResultPtrFromIObject(result)->isError();
    CATCH
    return false;
}

bool Application::isWarningResult(vcc::IObject *result)
{
    TRY
        return getIResultPtrFromIObject(result)->isWarning();
    CATCH
    return false;
}

void Application::eraseResult(vcc::IObject *result)
{
    TRY
        removeIObjectAll(application->_Results, result);
    CATCH
}

std::shared_ptr<vcc::IObject> Application::createActionArgument(const ObjectType &objectType)
{
    TRY
        auto actionArgument = ObjectFactory::create(objectType);
        application->_ActionArguments.insert(actionArgument);
        return actionArgument;
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> Application::doFormAction(vcc::IObject *form, const int64_t &formProperty, vcc::IObject *argument)
{
    TRY
        if (form == nullptr)
            return nullptr;
        auto result = getIFormPtrFromIObject(form)->doAction(formProperty, argument != nullptr ? argument->sharedPtr() : nullptr);
        if (argument != nullptr)
            removeIObjectAll(application->_ActionArguments, argument);
        auto obj = getIObjectPtrFromIResult(result.get())->sharedPtr();
        application->_Results.insert(obj);
        return obj;
    CATCH
    return nullptr;
}

int64_t Application::getFormActionCurrentSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return getIFormPtrFromIObject(form)->getActionCurrentSeqNo();
    CATCH
    return -1;
}

int64_t Application::getFormActionFirstSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return getIFormPtrFromIObject(form)->getActionFirstSeqNo();
    CATCH
    return -1;
}

int64_t Application::getFormActionLastSeqNo(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return getIFormPtrFromIObject(form)->getActionLastSeqNo();
    CATCH
    return -1;
}

void Application::redoFormAction(vcc::IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return;
        getIObjectPtrFromIResult(getIFormPtrFromIObject(form)->redoAction(noOfStep).get())->sharedPtr();
    CATCH
}

void Application::redoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return;
        getIObjectPtrFromIResult(getIFormPtrFromIObject(form)->redoActionToSeqNo(seqNo).get())->sharedPtr();
    CATCH
}

void Application::undoFormAction(vcc::IObject *form, const int64_t &noOfStep)
{
    TRY
        if (form == nullptr)
            return;
        getIObjectPtrFromIResult(getIFormPtrFromIObject(form)->undoAction(noOfStep).get())->sharedPtr();
    CATCH
}

void Application::undoFormActionToSeqNo(vcc::IObject *form, const int64_t &seqNo)
{
    TRY
        if (form == nullptr)
            return;
        getIObjectPtrFromIResult(getIFormPtrFromIObject(form)->undoActionToSeqNo(seqNo).get())->sharedPtr();
    CATCH
}

int64_t Application::clearFormAction(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return getIFormPtrFromIObject(form)->clearAction();
    CATCH
    return -1;
}

int64_t Application::truncateFormAction(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return -1;
        return getIFormPtrFromIObject(form)->truncateAction();
    CATCH
    return -1;
}

bool Application::isFormClosable(vcc::IObject *form)
{
    if (form == nullptr)
        return true;
    
    TRY
        return getIFormPtrFromIObject(form)->isClosable();
    CATCH
    return false;
}

bool Application::isFormClosed(vcc::IObject *form)
{
    TRY
        if (form == nullptr)
            return true;
        return getIFormPtrFromIObject(form)->isClosed();
    CATCH
    return true;
}

bool Application::closeForm(vcc::IObject *form, const bool &isForce)
{
    if (form == nullptr)
        return true;
    
    TRY
        if (getIFormPtrFromIObject(form)->Close(isForce)) {
            if (getFormSharedPtr(form) != nullptr)
                application->_Forms.erase(getFormSharedPtr(form));
            return true;
        }
    CATCH
    return false;
}

// <vcc:customApplicationFunctions sync="RESERVE" gen="RESERVE">
// </vcc:customApplicationFunctions>