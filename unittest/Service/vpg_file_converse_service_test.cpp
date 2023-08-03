#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "vpg_file_converse_service.hpp"

using namespace std;
using namespace vcc;

class FileConverseServiceTest : public testing::Test 
{
    GETOBJ(LogProperty, LogPropery);
    GET(wstring, Workspace, L"bin/Debug/Workspace/");

    public:

        void SetUp() override
        {
            this->_LogPropery.SetIsConsoleLog(false);
        }

        void TearDown() override
        {

        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPath(this->GetWorkspace(), path));
        }
};


TEST_F(FileConverseServiceTest, CheckAndCreateDirectory)
{
    VPGFileConverseService::CheckAndCreateDirectory(*this->GetLogPropery(), this->GetWorkspace());

    // bin
    EXPECT_TRUE(this->CheckFolderExists(L"bin"));
    EXPECT_TRUE(this->CheckFolderExists(L"bin/Debug"));
    EXPECT_TRUE(this->CheckFolderExists(L"bin/Release"));

    // include
    EXPECT_TRUE(this->CheckFolderExists(L"include"));

    // src
    EXPECT_TRUE(this->CheckFolderExists(L"src"));

    // unittest
    EXPECT_TRUE(this->CheckFolderExists(L"unittest"));
}
