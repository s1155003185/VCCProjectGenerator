#include <gtest/gtest.h>

#include "git_manager.hpp"

using namespace vcc;

class GitManagerTest : public testing::Test 
{
    private:
        MANAGER_SPTR(GitManager, Manager, nullptr);

    public:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
};

TEST_F(GitManagerTest, Full)
{

}
