#include "application.hpp"

#include <assert.h>
#include <map>

#include "base_form.hpp"
#include "exception_macro.hpp"
#include "object_factory.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="SKIP" gen="SKIP">
// </vcc:customHeader>

using namespace vcc;

void Application::Run()
{
    TRY
        if (application == nullptr)
            application = std::make_unique<Application>();
    CATCH
}

int64_t Application::Run(std::shared_ptr<IForm> form)
{
    TRY
        Run();

        application->_Forms.insert(std::make_pair(application->_NextFormId, form));
        application->_NextFormId++;
        return application->_NextFormId - 1;
    CATCH
    return -1;
}

int64_t Application::NewForm(ObjectType formType)
{
    TRY
        auto form = std::dynamic_pointer_cast<IForm>(ObjectFactory::Create(formType));
        assert(form != nullptr);
        _Forms.insert(std::make_pair(_NextFormId, form));
        _NextFormId++;
        return _NextFormId - 1;
    CATCH
    return -1;
}

bool Application::IsFormPresent(int64_t formId)
{
    TRY
        return _Forms.find(formId) != _Forms.end();
    CATCH
    return -1;
}

bool Application::IsFormClosable(int64_t formId)
{
    TRY
        if (!IsFormPresent(formId))
            return true;
        return _Forms[formId]->IsClosable();
    CATCH
    return false;
}

bool Application::CloseForm(int64_t formId, bool isForce)
{
    TRY
        if (!IsFormPresent(formId))
            return true;
        if (_Forms[formId]->OnClose(isForce)) {
            _Forms.erase(formId);
            return true;
        }
    CATCH
    return false;
}

// <vcc:custom sync="SKIP" gen="SKIP">
// </vcc:custom>