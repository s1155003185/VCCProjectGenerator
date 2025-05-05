#include <gtest/gtest.h>

#include "git_manager.hpp"

using namespace vcc;

class GitManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR_NULL(GitManager, Manager);

    public:
        void SetUp() override
        {
            _Manager = std::make_shared<GitManager>(nullptr);
        }

        void TearDown() override
        {
        }
};

TEST_F(GitManagerTest, Full)
{

}
