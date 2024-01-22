#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "log_property.hpp"
#include "memory_macro.hpp"

using namespace std;
using namespace vcc;

class GitServiceTest : public testing::Test 
{
    GETSPTR(LogProperty, LogProperty);
    GET(wstring, Workspace, L"bin/Debug/Git/");
    public:

        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
        }

        void TearDown() override
        {

        }
};

TEST_F(GitServiceTest, Version)
{
    std::wstring version = GitService::GetVersion(this->GetLogProperty().get());
    EXPECT_TRUE(regex_match(version, wregex(L"[0-9]+.[0-9]+.[0-9]+")));
}

TEST_F(GitServiceTest, Config)
{
    DECLARE_SPTR(GitConfig, config);
    GitService::GetGlobalConfig(this->GetLogProperty().get(), config);
    std::wstring userName = GitService::GetGlobalUserName(this->GetLogProperty().get());
    std::wstring userEmail = GitService::GetGlobalUserEmail(this->GetLogProperty().get());
    EXPECT_TRUE(!config->GetUserName().empty());
    EXPECT_TRUE(!config->GetUserEmail().empty());
    EXPECT_EQ(config->GetUserName(), userName);
    EXPECT_EQ(config->GetUserEmail(), userEmail);
}

TEST_F(GitServiceTest, ParseGitDiff)
{
    std::wstring str = L"diff --git a/test.txt b/test.txt\r\n";
    str += L"index edf0eff..ab966a8 100644\r\n";
    str += L"--- a/test.txt\r\n";
    str += L"+++ b/test.txt\r\n";
    str += L"@@ -1 +1,2 @@\r\n";
    str += L" hi\r\n";
    str += L"+HI\r\n";
    std::wstring expectedChangedLine = L"";
    expectedChangedLine += L" hi\r\n";
    expectedChangedLine += L"+HI\r\n";
    DECLARE_SPTR(GitDifference, diff);
    GitService::ParseGitDiff(str, diff);
    EXPECT_EQ(diff->GetFilePathOld(), L"test.txt");
    EXPECT_EQ(diff->GetFilePathNew(), L"test.txt");
    EXPECT_EQ(diff->GetLineNumberOld()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountOld()[0], (size_t)0);
    EXPECT_EQ(diff->GetLineNumberNew()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountNew()[0], (size_t)2);
    EXPECT_EQ(diff->GetChangedLines()[0], expectedChangedLine);
}

TEST_F(GitServiceTest, LogAndBranch)
{

}

TEST_F(GitServiceTest, StageAndDifference)
{
    if (std::filesystem::exists(this->GetWorkspace()))
        std::filesystem::remove_all(this->GetWorkspace());
    std::filesystem::create_directory(this->GetWorkspace());

    // init
    GitService::Initialize(this->GetLogProperty().get(), this->GetWorkspace());
    EXPECT_TRUE(filesystem::exists(this->GetWorkspace() + L"/.git/HEAD"));
    
    // check existance
    EXPECT_TRUE(GitService::IsGitResponse(this->GetLogProperty().get(), this->GetWorkspace()));

    // config
    DECLARE_SPTR(GitConfig, config);
    GitService::GetLocalConfig(this->GetLogProperty().get(), this->GetWorkspace(), config);
    EXPECT_TRUE(config->GetUserName().empty());
    EXPECT_TRUE(config->GetUserEmail().empty());
    EXPECT_FALSE(GitService::IsLocalConfigExists(this->GetLogProperty().get(), this->GetWorkspace(), L"user.name"));
    
    GitService::SetLocalUserName(this->GetLogProperty().get(), this->GetWorkspace(), L"test");
    GitService::SetLocalUserEmail(this->GetLogProperty().get(), this->GetWorkspace(), L"test@test.com");
    EXPECT_EQ(GitService::GetLocalUserName(this->GetLogProperty().get(), this->GetWorkspace()), L"test");
    EXPECT_EQ(GitService::GetLocalUserEmail(this->GetLogProperty().get(), this->GetWorkspace()), L"test@test.com");
    EXPECT_EQ(GitService::GetUserName(this->GetLogProperty().get(), this->GetWorkspace()), L"test");
    EXPECT_EQ(GitService::GetUserEmail(this->GetLogProperty().get(), this->GetWorkspace()), L"test@test.com");
    EXPECT_TRUE(GitService::IsLocalConfigExists(this->GetLogProperty().get(), this->GetWorkspace(), L"user.name"));

    // Case: Empty File
    DECLARE_SPTR(GitStatus, statusEmpty);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusEmpty);
    EXPECT_EQ(statusEmpty->GetBranch(), L"main");
    EXPECT_EQ(statusEmpty->GetRemoteBranch(), L"");

    // Case: New File
    WriteFile(ConcatPath(this->GetWorkspace(), L"test.txt"), L"hi\r\n", true);
    DECLARE_SPTR(GitStatus, statusCreateFile);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusCreateFile);
    EXPECT_EQ(statusCreateFile->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test.txt");

    // Case: Staged
    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
    DECLARE_SPTR(GitStatus, statusNewState);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusNewState);
    EXPECT_EQ(statusNewState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusNewState->GetIndexFiles()[GitFileStatus::Added].size(), (size_t)1);
    EXPECT_EQ(statusNewState->GetIndexFiles()[GitFileStatus::Added].at(0), L"test.txt");
    EXPECT_EQ(statusNewState->GetWorkingTreeFiles().size(), (size_t)0);
    GitService::Unstage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
    GitService::StageAll(this->GetLogProperty().get(), this->GetWorkspace());
    GitService::UnstageAll(this->GetLogProperty().get(), this->GetWorkspace());
    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
 
    // Case: Committed
    GitService::Commit(this->GetLogProperty().get(), this->GetWorkspace(), L"Test Commit");
    DECLARE_SPTR(GitStatus, statusNewCommit);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusNewCommit);
    EXPECT_EQ(statusNewCommit->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusNewCommit->GetWorkingTreeFiles().size(), (size_t)0);
    
    // Case: Modified
    AppendFileOneLine(ConcatPath(this->GetWorkspace(), L"test.txt"), L"HI", false);
    DECLARE_SPTR(GitStatus, statusModify);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusModify);
    EXPECT_EQ(statusModify->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    DECLARE_SPTR(GitDifferenceSummary, differentSummary);
    GitService::GetDifferenceSummary(this->GetLogProperty().get(), this->GetWorkspace(), {}, differentSummary);
    EXPECT_EQ(differentSummary->GetFiles().size(), (size_t)1);
    EXPECT_EQ(differentSummary->GetFiles().at(0), L"test.txt");
    EXPECT_EQ(differentSummary->GetAddLineCounts().at(0), (size_t)1);
    EXPECT_EQ(differentSummary->GetDeleteLineCounts().at(0), (size_t)0);
    DECLARE_SPTR(GitDifference, diff);
    GitService::GetDifferenceWorkingFile(this->GetLogProperty().get(), this->GetWorkspace(), {}, L"test.txt", diff); // output in ParseGitDiff
    EXPECT_EQ(diff->GetFilePathOld(), L"test.txt");
    EXPECT_EQ(diff->GetFilePathNew(), L"test.txt");
    EXPECT_EQ(diff->GetLineNumberOld()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountOld()[0], (size_t)0);
    EXPECT_EQ(diff->GetLineNumberNew()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountNew()[0], (size_t)2);
    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
    DECLARE_SPTR(GitStatus, statusModifyState);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusModifyState);
    EXPECT_EQ(statusModifyState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState->GetIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState->GetIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState->GetWorkingTreeFiles().size(), (size_t)0);
    AppendFileOneLine(ConcatPath(this->GetWorkspace(), L"test.txt"), L"BI", false);
    DECLARE_SPTR(GitStatus, statusModifyState2);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusModifyState2);
    EXPECT_EQ(statusModifyState2->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");

    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
    GitService::Commit(this->GetLogProperty().get(), this->GetWorkspace(), L"Test Modify");

    // Case: Renamed
    std::filesystem::rename(ConcatPath(this->GetWorkspace(), L"test.txt"), ConcatPath(this->GetWorkspace(), L"test2.txt"));
    DECLARE_SPTR(GitStatus, statusRename);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusRename);
    EXPECT_EQ(statusRename->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles().size(), (size_t)2);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test2.txt");
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test.txt");
    GitService::StageAll(this->GetLogProperty().get(), this->GetWorkspace());
    DECLARE_SPTR(GitStatus, statusRenameState);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusRenameState);
    EXPECT_EQ(statusRenameState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusRenameState->GetIndexFiles()[GitFileStatus::Renamed].size(), (size_t)1);
    EXPECT_EQ(statusRenameState->GetIndexFiles()[GitFileStatus::Renamed].at(0), L"test.txt -> test2.txt");
    EXPECT_EQ(statusRenameState->GetWorkingTreeFiles().size(), (size_t)0);
    GitService::Commit(this->GetLogProperty().get(), this->GetWorkspace(), L"Test Rename");

    // Case: Deleted
    RemoveFile(ConcatPath(this->GetWorkspace(), L"test2.txt"));
    DECLARE_SPTR(GitStatus, statusDelete);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusDelete);
    EXPECT_EQ(statusDelete->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test2.txt");
    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test2.txt");
    GitService::Commit(this->GetLogProperty().get(), this->GetWorkspace(), L"Test Delete");
}
