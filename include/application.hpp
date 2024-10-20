// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <map>
#include <memory>

#include "i_form.hpp"
#include "object_type.hpp"

// <vcc:customHeader sync="SKIP" gen="SKIP">
// </vcc:customHeader>

using namespace vcc;

class Application
{
    private:
        int64_t _NextFormId = 0;
        std::map<int64_t, std::shared_ptr<IForm>> _Forms;

    public:
        Application() = default;
        ~Application() {}
 
        static void Run();
        static int64_t Run(std::shared_ptr<IForm> form);

        int64_t NewForm(ObjectType formType);
        bool IsFormPresent(int64_t formId);
        bool IsFormClosable(int64_t formId);
        bool CloseForm(int64_t formId, bool isForce = false);

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // </vcc:custom>
};

static std::unique_ptr<Application> application = nullptr;