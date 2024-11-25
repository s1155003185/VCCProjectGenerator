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
        application->_Forms.insert(form);
        return form;
    CATCH
    return nullptr;
}

void Application::InitializeForm(IObject *form)
{
    TRY
        if (form == nullptr)
            return;
        GetIFormPtrFromIObject(form)->Initialize();
    CATCH
}

void Application::ReloadForm(IObject *form)
{
    TRY
        if (form == nullptr)
            return;
        GetIFormPtrFromIObject(form)->Reload();
    CATCH
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