#include <gtest/gtest.h>

#include "application.hpp"
#include "base_form.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"
#include "lock_type.hpp"
#include "property_accessor_factory.hpp"
#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"

// Mode
TEST(ApplicationTest, CreateForm)
{
    Application::Run();
    // auto form =  ApplicationCreateForm((int64_t)ObjectType::GitForm);
    // ReadObject(form, (int64_t)VPGGitFormProperty::Log, -1);

    auto form = Application::createForm(ObjectType::MainForm);
    auto gitform = static_cast<vcc::BaseForm *>(form.get());
    vcc::IObject *object = static_cast<vcc::IObject *>((gitform));
    EXPECT_TRUE(object != nullptr);
}
