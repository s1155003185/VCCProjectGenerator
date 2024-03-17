#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "vpg_project_config_manager.hpp"

using namespace std;
using namespace vcc;

class VPGProjectConfigManagerTest : public testing::Test 
{
    GET(wstring, Workspace, L"bin/Debug/VPGProjectConfigManagerTest/");

    MANAGER(VPGProjectConfigManager, Manager);

    public:
        void SetUp() override
        {
            filesystem::remove_all(PATH(this->GetWorkspace()));

            CreateDirectory(this->GetWorkspace());
        }

        void TearDown() override
        {
            // ensure compile main project will not compile this project
            filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGProjectConfigManagerTest, Normal)
{
    
}
