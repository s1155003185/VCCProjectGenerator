#include <gtest/gtest.h>

#include "git_manager.hpp"

class GitManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR_NULL(vcc::GitManager, Manager);

    public:
        void SetUp() override
        {
            _Manager = std::make_shared<vcc::GitManager>(nullptr);
        }

        void TearDown() override
        {
        }
};

TEST_F(GitManagerTest, Full)
{

}
