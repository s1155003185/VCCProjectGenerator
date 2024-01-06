#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

using namespace std;
using namespace vcc;

class FileTest : public testing::Test 
{
    GETUPTR(LogProperty, LogProperty);
    GET(wstring, Workspace, L"bin/Debug/FileTest/");
    GET(wstring, WorkspaceSource, L"bin/Debug/FileTest/WorkspaceSource");
    GET(wstring, WorkspaceTarget, L"bin/Debug/FileTest/WorkspaceTarget");

    GET(wstring, FilePathSourceA, L"bin/Debug/FileTest/WorkspaceSource/FileA.txt");
    GET(wstring, FilePathTargetB, L"bin/Debug/FileTest/WorkspaceTarget/FileB.txt");
    GET(wstring, FilePathSourceC, L"bin/Debug/FileTest/WorkspaceSource/FileC.txt");
    GET(wstring, FilePathTargetC, L"bin/Debug/FileTest/WorkspaceTarget/FileC.txt");


    public:

        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            filesystem::remove_all(PATH(this->GetWorkspace()));
            CreateDirectory(this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            // FIle A
            AppendFileOneLine(this->GetFilePathSourceA(), L"File A", true);
            // File B
            AppendFileOneLine(this->GetFilePathTargetB(), L"File B", true);
            // File C
            AppendFileOneLine(this->GetFilePathSourceC(), L"File C Source", true);
            AppendFileOneLine(this->GetFilePathTargetC(), L"File C Target", true);

        }

        void TearDown() override
        {

        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPath(this->GetWorkspace(), path));
        }
};

TEST_F(FileTest, GetFileDifferenceBetweenWorkspacesTest)
{
    std::vector<std::wstring> needToAdd;
    std::vector<std::wstring> needToDelete;
    std::vector<std::wstring> needToModify;
    GetFileDifferenceBetweenWorkspaces(this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
        needToAdd, needToModify, needToDelete);
    EXPECT_EQ((int)needToAdd.size(), 1);
    EXPECT_TRUE((int)needToAdd.at(0).ends_with(L"FileA.txt"));
    EXPECT_EQ((int)needToDelete.size(), 1);
    EXPECT_TRUE((int)needToDelete.at(0).ends_with(L"FileB.txt"));
    EXPECT_EQ((int)needToModify.size(), 1);
    EXPECT_TRUE((int)needToModify.at(0).ends_with(L"FileC.txt"));
}
