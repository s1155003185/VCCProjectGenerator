// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <set>
#include <memory>

#include "i_form.hpp"
#include "i_object.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class Application
{
    private:
        std::set<std::shared_ptr<IObject>> _Forms;

        static std::shared_ptr<IObject> GetFormSharedPtr(IObject *IObject);
        static const IForm *GetIFormPtrFromIObject(IObject *obj);

        // <vcc:customApplicationProperties sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProperties>
        
    private:
        // <vcc:customApplicationPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPrivateFunctions>

    protected:
        // <vcc:customApplicationProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationProtectedFunctions>

    public:
        Application() = default;
        ~Application() {}
 
        static void Run();

        // Create Form
        static std::shared_ptr<IObject> CreateForm(const ObjectType &objectType);
        static void InitializeForm(IObject *form);
        static void ReloadForm(IObject *form);

        // Close Form
        static bool IsFormClosable(IObject *form);
        static bool IsFormClosed(IObject *form);
        static bool CloseForm(IObject *form, const bool &isForce = false);

        // <vcc:customApplicationPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customApplicationPublicFunctions>
};

static std::unique_ptr<Application> application = nullptr;