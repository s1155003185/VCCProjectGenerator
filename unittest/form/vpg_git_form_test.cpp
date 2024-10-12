#include <gtest/gtest.h>

#include "vpg_git_form.hpp"

class VPGGitFormTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig, LogConfigInitialType::None);
    std::shared_ptr<VPGGitForm> _Form = nullptr;
    public:
        VPGGitFormTest() {}
        virtual ~VPGGitFormTest() {}

        void SetUp() override
        {
            _Form = std::make_shared<VPGGitForm>(_LogConfig);
        }

        void TearDown() override
        {
        }
};

TEST_F(VPGGitFormTest, Normal)
{
    
}
