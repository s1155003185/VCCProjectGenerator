#include <gtest/gtest.h>

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"

using namespace vcc;

class FileHelperTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/FileHelperTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");

    GETSET(std::wstring, FilePathSourceA, L"");
    GETSET(std::wstring, FilePathTargetB, L"");
    GETSET(std::wstring, FilePathSourceC, L"");
    GETSET(std::wstring, FilePathTargetC, L"");


    public:

        void SetUp() override
        {
            this->_LogConfig = std::make_shared<LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";
            this->_FilePathSourceA = ConcatPaths({this->_WorkspaceSource, L"FileA.txt"});
            this->_FilePathTargetB = ConcatPaths({this->_WorkspaceTarget, L"FileB.txt"});
            this->_FilePathSourceC = ConcatPaths({this->_WorkspaceSource, L"FileC.txt"});
            this->_FilePathTargetC = ConcatPaths({this->_WorkspaceTarget, L"FileC.txt"});
            
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
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
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPaths({this->GetWorkspace(), path}));
        }
};

TEST_F(FileHelperTest, GetRelativePath)
{
    std::wstring absoluttePath = L"/abc/def/ghi.text";
    std::wstring basePath = L"/abc";
    EXPECT_EQ(GetLinuxPath(GetRelativePath(absoluttePath, basePath)), L"def/ghi.text");

    absoluttePath = L"/abc/def/ghi.text";
    basePath = L"/abc/";
    EXPECT_EQ(GetLinuxPath(GetRelativePath(absoluttePath, basePath)), L"def/ghi.text");
}

TEST_F(FileHelperTest, GetFileDifferenceBetweenWorkspacesTest)
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

TEST_F(FileHelperTest, GetRegexFromFileFilter)
{
    EXPECT_EQ(GetRegexFromFileFilter(L"*.txt"), L".*\\.txt");
    EXPECT_EQ(GetRegexFromFileFilter(L"*abc*.txt"), L".*abc.*\\.txt");

    EXPECT_TRUE(std::regex_match(L"abcdef.txt", std::wregex(GetRegexFromFileFilter(L"*def*"))));
}

TEST_F(FileHelperTest, CopyDirectoryWithoutFilter)
{
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    CreateDirectory(this->GetWorkspaceTarget());
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    CopyDirectoryOption option;
    option.SetIsRecursive(true);
    CopyDirectory(this->GetWorkspaceSource(), this->GetWorkspaceTarget(), &option);
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}

TEST_F(FileHelperTest, CopyDirectoryWithIncludeFilter)
{
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    CreateDirectory(this->GetWorkspaceTarget());
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    CopyDirectoryOption option;
    option.SetIsRecursive(true);
    option.InsertIncludeFileFilters(L"*FileA*");
    CopyDirectory(this->GetWorkspaceSource(), this->GetWorkspaceTarget(), &option);
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}

TEST_F(FileHelperTest, CopyDirectoryWithExcludeFilter)
{
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    CreateDirectory(this->GetWorkspaceTarget());
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    CopyDirectoryOption option;
    option.SetIsRecursive(true);
    option.InsertExcludeFileFilters(L"*FileA*");
    CopyDirectory(this->GetWorkspaceSource(), this->GetWorkspaceTarget(), &option);
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}