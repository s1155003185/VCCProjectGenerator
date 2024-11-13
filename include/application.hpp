// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <set>
#include <memory>

#include "i_form.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class Application
{
    private:
        std::set<std::shared_ptr<IForm>> _Forms;

    public:
        Application() = default;
        ~Application() {}
 
        static void Run();

        static std::shared_ptr<IForm> CreateForm(const ObjectType &objectType);
        static std::shared_ptr<IForm> GetFormSharedPtr(const IForm *form);
        static bool IsFormPresent(const IForm *form);
        static bool IsFormClosable(const IForm *form);
        static bool CloseForm(const IForm *form, const bool &isForce = false);

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // </vcc:custom>
};

static std::unique_ptr<Application> application = nullptr;