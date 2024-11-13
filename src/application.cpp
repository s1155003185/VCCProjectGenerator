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

void Application::Run()
{
    TRY
        if (application == nullptr)
            application = std::make_unique<Application>();
    CATCH
}

std::shared_ptr<IForm> Application::CreateForm(const ObjectType &objectType)
{
    TRY
        auto form = std::dynamic_pointer_cast<IForm>(ObjectFactory::Create(objectType));
        assert(form != nullptr);
        application->_Forms.insert(form);
        return form;
    CATCH
    return nullptr;
}

std::shared_ptr<IForm> Application::GetFormSharedPtr(const IForm *form)
{
    TRY
        for (auto tmpForm : application->_Forms) {
            if (tmpForm.get() == form)
                return tmpForm;
        }
    CATCH
    return nullptr;
}

bool Application::IsFormPresent(const IForm *form)
{
    if (form == nullptr)
        return false;
    
    TRY
        return GetFormSharedPtr(form) != nullptr;
    CATCH
    return false;
}

bool Application::IsFormClosable(const IForm *form)
{
    if (form == nullptr)
        return true;
    
    TRY
        if (!IsFormPresent(form))
            return true;
        return form->IsClosable();
    CATCH
    return false;
}

bool Application::CloseForm(const IForm *form, const bool &isForce)
{
    if (form == nullptr)
        return true;
    
    TRY
        if (!IsFormPresent(form))
            return true;
        if (form->OnClose(isForce)) {            
            application->_Forms.erase(GetFormSharedPtr(form));
            return true;
        }
    CATCH
    return false;
}

// <vcc:custom sync="RESERVE" gen="RESERVE">
// </vcc:custom>