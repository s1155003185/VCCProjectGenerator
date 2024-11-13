#include <gtest/gtest.h>

#include "application.hpp"
//#include "DllFunctions.h"
#include "i_object.hpp"
#include "i_property_accessor.hpp"
#include "lock_type.hpp"
#include "property_accessor_factory.hpp"
#include "vpg_git_form_property.hpp"
#include "vpg_git_form.hpp"

using namespace vcc;

// Mode
TEST(ApplicationTest, CreateForm)
{
    Application::Run();
    // auto form =  ApplicationCreateForm((int64_t)ObjectType::GitForm);
    // ReadObject(form, (int64_t)VPGGitFormProperty::Log, -1);

    auto form = Application::CreateForm(ObjectType::GitForm);
    VPGGitForm *gitform = static_cast<VPGGitForm *>(form.get());
    IObject *object = static_cast<IObject *>((gitform));
    //IObject *object =  form.get();
    std::shared_ptr<IObject> ptr(object, [](IObject*){});
    std::shared_ptr<IObject> result = PropertyAccessorFactory::Create(ptr)->ReadObject(LockType::ReadLock, 0, -1);
}
