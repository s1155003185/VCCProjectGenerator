#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "class_macro.hpp"
#include "vpg_process_manager.hpp"

using namespace vcc;

class VPGProcessManagerTest : public testing::Test 
{
    GETUPTR(VPGProcessManager, Manager);

    public:

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }
};

TEST_F(VPGProcessManagerTest, Empty)
{
    std::vector<std::wstring> cmds;
    this->GetManager()->Execute(cmds);
}

TEST_F(VPGProcessManagerTest, AddCpp)
{
    

}
