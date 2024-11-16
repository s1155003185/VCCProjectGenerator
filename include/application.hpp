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

    public:
        Application() = default;
        ~Application() {}
 
        static void Run();

        static std::shared_ptr<IObject> CreateForm(const ObjectType &objectType);
        static std::shared_ptr<IObject> GetFormSharedPtr(IObject *IObject);
        static const IForm *GetIFormPtrFromIObject(IObject *obj);
        static bool IsFormPresent(IObject *form);
        static bool IsFormClosable(IObject *form);
        static bool CloseForm(IObject *form, const bool &isForce = false);

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // </vcc:custom>
};

static std::unique_ptr<Application> application = nullptr;