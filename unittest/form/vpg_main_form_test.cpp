#include <gtest/gtest.h>

#include <memory>

#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"

TEST(VPGMainFormTest, AddDeleteWorkspaceForm)
{
    auto form = std::make_shared<VPGMainForm>();
    form->DoAction(static_cast<int64_t>(VPGMainFormProperty::AddWorkspaceForm));
    EXPECT_EQ(form->GetWorkspaceForms().size(), (size_t)1);
    form->DoAction(static_cast<int64_t>(VPGMainFormProperty::DeleteWorkspaceForm));
    EXPECT_TRUE(form->GetWorkspaceForms().empty());
}
