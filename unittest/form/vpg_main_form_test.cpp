#include <gtest/gtest.h>

#include <memory>

#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"

TEST(VPGMainFormTest, AddDeleteWorkspaceForm_Property)
{
    auto form = std::make_shared<VPGMainForm>();
    form->insertWorkspaceForms(std::make_shared<VPGWorkspaceForm>());
    EXPECT_EQ(form->getWorkspaceForms().size(), (size_t)1);
    form->removeWorkspaceForms(form->getWorkspaceFormsAtIndex(0).get());
    EXPECT_TRUE(form->getWorkspaceForms().empty());
}
