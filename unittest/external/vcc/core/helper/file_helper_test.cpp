#include <gtest/gtest.h>

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"

class FileHelperTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/FileHelperTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");

    GETSET(std::wstring, FilePathSourceA, L"");
    GETSET(std::wstring, FilePathTargetB, L"");
    GETSET(std::wstring, FilePathSourceC, L"");
    GETSET(std::wstring, FilePathTargetC, L"");


    public:

        void setUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";
            this->_FilePathSourceA = vcc::ConcatPaths({this->_WorkspaceSource, L"FileA.txt"});
            this->_FilePathTargetB = vcc::ConcatPaths({this->_WorkspaceTarget, L"FileB.txt"});
            this->_FilePathSourceC = vcc::ConcatPaths({this->_WorkspaceSource, L"FileC.txt"});
            this->_FilePathTargetC = vcc::ConcatPaths({this->_WorkspaceTarget, L"FileC.txt"});
            
            std::filesystem::remove_all(PATH(this->getWorkspace()));
            vcc::CreateDirectory(this->getWorkspace());
            vcc::CreateDirectory(this->getWorkspaceSource());
            vcc::CreateDirectory(this->getWorkspaceTarget());

            // FIle A
            vcc::AppendFileOneLine(this->getFilePathSourceA(), L"File A", true);
            // File B
            vcc::AppendFileOneLine(this->getFilePathTargetB(), L"File B", true);
            // File C
            vcc::AppendFileOneLine(this->getFilePathSourceC(), L"File C Source", true);
            vcc::AppendFileOneLine(this->getFilePathTargetC(), L"File C Target", true);

        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }

        bool CheckFolderExists(std::wstring path)
        {
            return vcc::IsDirectoryExists(vcc::ConcatPaths({this->getWorkspace(), path}));
        }
};

TEST_F(FileHelperTest, getRelativePath)
{
    std::wstring absoluttePath = L"/abc/def/ghi.text";
    std::wstring basePath = L"/abc";
    EXPECT_EQ(vcc::GetLinuxPath(vcc::GetRelativePath(absoluttePath, basePath)), L"def/ghi.text");

    absoluttePath = L"/abc/def/ghi.text";
    basePath = L"/abc/";
    EXPECT_EQ(vcc::GetLinuxPath(vcc::GetRelativePath(absoluttePath, basePath)), L"def/ghi.text");
}

TEST_F(FileHelperTest, getFileDifferenceBetweenWorkspacesTest)
{
    std::vector<std::wstring> needToAdd;
    std::vector<std::wstring> needToDelete;
    std::vector<std::wstring> needToModify;
    vcc::GetFileDifferenceBetweenWorkspaces(this->getWorkspaceSource(), this->getWorkspaceTarget(),
        needToAdd, needToModify, needToDelete);
    EXPECT_EQ((int)needToAdd.size(), 1);
    EXPECT_TRUE((int)needToAdd.at(0).ends_with(L"FileA.txt"));
    EXPECT_EQ((int)needToDelete.size(), 1);
    EXPECT_TRUE((int)needToDelete.at(0).ends_with(L"FileB.txt"));
    EXPECT_EQ((int)needToModify.size(), 1);
    EXPECT_TRUE((int)needToModify.at(0).ends_with(L"FileC.txt"));
}

TEST_F(FileHelperTest, getRegexFromFileFilter)
{
    EXPECT_EQ(vcc::GetRegexFromFileFilter(L"*.txt"), L".*\\.txt");
    EXPECT_EQ(vcc::GetRegexFromFileFilter(L"*abc*.txt"), L".*abc.*\\.txt");

    EXPECT_TRUE(std::regex_match(L"abcdef.txt", std::wregex(vcc::GetRegexFromFileFilter(L"*def*"))));
}

TEST_F(FileHelperTest, CopyDirectoryWithoutFilter)
{
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    vcc::CreateDirectory(this->getWorkspaceTarget());
    vcc::AppendFileOneLine(vcc::ConcatPaths({this->getWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    vcc::CopyDirectoryOption option;
    option.SetIsRecursive(true);
    CopyDirectory(this->getWorkspaceSource(), this->getWorkspaceTarget(), &option);
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}

TEST_F(FileHelperTest, CopyDirectoryWithIncludeFilter)
{
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    vcc::CreateDirectory(this->getWorkspaceTarget());
    vcc::AppendFileOneLine(vcc::ConcatPaths({this->getWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    vcc::CopyDirectoryOption option;
    option.SetIsRecursive(true);
    option.InsertIncludeFileFilters(L"*FileA*");
    CopyDirectory(this->getWorkspaceSource(), this->getWorkspaceTarget(), &option);
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}

TEST_F(FileHelperTest, CopyDirectoryWithExcludeFilter)
{
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    vcc::CreateDirectory(this->getWorkspaceTarget());
    vcc::AppendFileOneLine(vcc::ConcatPaths({this->getWorkspaceSource(), L"FolderA", L"FileA.txt"}), L"File A", true);
    vcc::CopyDirectoryOption option;
    option.SetIsRecursive(true);
    option.InsertExcludeFileFilters(L"*FileA*");
    CopyDirectory(this->getWorkspaceSource(), this->getWorkspaceTarget(), &option);
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileA.txt"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FileC.txt"})));
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->getWorkspaceTarget(), L"FolderA", L"FileA.txt"})));
}