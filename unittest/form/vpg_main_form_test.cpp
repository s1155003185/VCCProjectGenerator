#include <gtest/gtest.h>

#include <memory>

#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"

TEST(VPGMainFormTest, AddDeleteWorkspaceForm_Property)
{
    auto form = std::make_shared<VPGMainForm>();
    form->InsertWorkspaceForms(std::make_shared<VPGWorkspaceForm>());
    EXPECT_EQ(form->GetWorkspaceForms().size(), (size_t)1);
    form->RemoveWorkspaceForms(form->GetWorkspaceFormsAtIndex(0).get());
    EXPECT_TRUE(form->GetWorkspaceForms().empty());
}

TEST(VPGMainFormTest, AddDeleteWorkspaceForm_Action)
{
    auto form = std::make_shared<VPGMainForm>();
    form->DoAction(static_cast<int64_t>(VPGMainFormProperty::AddWorkspaceForm));
    EXPECT_EQ(form->GetWorkspaceForms().size(), (size_t)1);
    form->DoAction(static_cast<int64_t>(VPGMainFormProperty::DeleteWorkspaceForm));
    EXPECT_TRUE(form->GetWorkspaceForms().empty());
}
