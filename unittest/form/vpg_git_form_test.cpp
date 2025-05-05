#include <gtest/gtest.h>

#include "vpg_git_form.hpp"

class VPGGitFormTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    std::shared_ptr<VPGGitForm> _Form = nullptr;
    public:
        VPGGitFormTest() {}
        virtual ~VPGGitFormTest() {}

        void SetUp() override
        {
            _LogConfig = std::make_shared<LogConfig>(LogConfigInitialType::None);
            _Form = std::make_shared<VPGGitForm>();
        }

        void TearDown() override
        {
        }
};

TEST_F(VPGGitFormTest, Normal)
{
    
}
