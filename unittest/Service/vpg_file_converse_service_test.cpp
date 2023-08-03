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
    GET(wstring, Workspace, L"bin/Debug/FileConverseServiceTest/");
    GET(wstring, WorkspaceSource, L"bin/Debug/FileConverseServiceTest/WorkspaceSource");
    GET(wstring, WorkspaceTarget, L"bin/Debug/FileConverseServiceTest/WorkspaceTarget");

    GET(wstring, FilePathSourceA, L"bin/Debug/FileConverseServiceTest/WorkspaceSource/FileA.txt");
    GET(wstring, FilePathTargetB, L"bin/Debug/FileConverseServiceTest/WorkspaceTarget/FileB.txt");
    GET(wstring, FilePathSourceC, L"bin/Debug/FileConverseServiceTest/WorkspaceSource/FileC.txt");
    GET(wstring, FilePathTargetC, L"bin/Debug/FileConverseServiceTest/WorkspaceTarget/FileC.txt");


    public:

        void SetUp() override
        {
            this->_LogPropery.SetIsConsoleLog(false);
            filesystem::remove_all(PATH(this->GetWorkspace()));
            CreateDirectory(this->GetWorkspace());

            VPGFileConverseService::CheckAndCreateDirectory(*this->GetLogPropery(), this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            // FIle A
            AppendFileSingleLine(this->GetFilePathSourceA(), L"File A", true);
            // File B
            AppendFileSingleLine(this->GetFilePathTargetB(), L"File B", true);
            // File C
            AppendFileSingleLine(this->GetFilePathSourceC(), L"File C Source", true);
            AppendFileSingleLine(this->GetFilePathTargetC(), L"File C Target", true);

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
