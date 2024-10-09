#include <gtest/gtest.h>

#include "vpg_git_form.hpp"

class VPGGitFormTest : public testing::Test 
{
    GETSET_SPTR(LogProperty, LogProperty, LogPropertyType::None);
    std::shared_ptr<VPGGitForm> _Form = nullptr;
    public:
        VPGGitFormTest() {}
        virtual ~VPGGitFormTest() {}

        void SetUp() override
        {
            _Form = std::make_shared<VPGGitForm>(_LogProperty);
        }

        void TearDown() override
        {
        }
};

TEST_F(VPGGitFormTest, Normal)
{
    
}
