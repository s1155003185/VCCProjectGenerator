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

            if (std::filesystem::exists(this->GetWorkspace()))
                std::filesystem::remove_all(this->GetWorkspace());
            std::filesystem::create_directory(this->GetWorkspace());
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

TEST_F(GitServiceTest, ParseGitLogGraph)
{
    std::wstring str = L"";
    str += L"* (A1)(A1Short)(A2)(A2Short)(A3)(A3Short)\n"; // normal
    str += L"*   (A3)(A3Short)(A4)(A4Short)(A5 A6)(A5Short A6Short)\n"; // merge
    str += L"|\n";
    str += L"| * (A6)(A6Short)(A7)(A7Short)(A8)(A8Short)\r\n"; // merge branch
    str += L"* (B1)(B1Short)(B2)(B2Short)()()\r\n"; // node init

    std::vector<std::shared_ptr<GitLog>> logs;
    GitService::ParseGitLogGraph(str, logs);
    EXPECT_EQ(logs.size(), (size_t)4);
    EXPECT_EQ(logs.at(0)->GetColumnIndex(), (size_t)0);
    EXPECT_EQ(logs.at(0)->GetHashID(), L"A1");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedHashID(), L"A1Short");
    EXPECT_EQ(logs.at(0)->GetTreeHashID(), L"A2");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedTreeHashID(), L"A2Short");
    EXPECT_EQ(logs.at(0)->GetParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->GetParentHashIDs().at(0), L"A3");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->GetAbbreviatedParentHashIDs().at(0), L"A3Short");

    EXPECT_EQ(logs.at(1)->GetColumnIndex(), (size_t)0);
    EXPECT_EQ(logs.at(1)->GetHashID(), L"A3");
    EXPECT_EQ(logs.at(1)->GetAbbreviatedHashID(), L"A3Short");
    EXPECT_EQ(logs.at(1)->GetTreeHashID(), L"A4");
    EXPECT_EQ(logs.at(1)->GetAbbreviatedTreeHashID(), L"A4Short");
    EXPECT_EQ(logs.at(1)->GetParentHashIDs().size(), (size_t)2);
    EXPECT_EQ(logs.at(1)->GetParentHashIDs().at(0), L"A5");
    EXPECT_EQ(logs.at(1)->GetParentHashIDs().at(1), L"A6");
    EXPECT_EQ(logs.at(1)->GetAbbreviatedParentHashIDs().size(), (size_t)2);
    EXPECT_EQ(logs.at(1)->GetAbbreviatedParentHashIDs().at(0), L"A5Short");
    EXPECT_EQ(logs.at(1)->GetAbbreviatedParentHashIDs().at(1), L"A6Short");

    EXPECT_EQ(logs.at(2)->GetColumnIndex(), (size_t)1);
    EXPECT_EQ(logs.at(2)->GetHashID(), L"A6");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedHashID(), L"A6Short");
    EXPECT_EQ(logs.at(2)->GetTreeHashID(), L"A7");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedTreeHashID(), L"A7Short");
    EXPECT_EQ(logs.at(2)->GetParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->GetParentHashIDs().at(0), L"A8");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->GetAbbreviatedParentHashIDs().at(0), L"A8Short");

    EXPECT_EQ(logs.at(3)->GetColumnIndex(), (size_t)0);
    EXPECT_EQ(logs.at(3)->GetHashID(), L"B1");
    EXPECT_EQ(logs.at(3)->GetAbbreviatedHashID(), L"B1Short");
    EXPECT_EQ(logs.at(3)->GetTreeHashID(), L"B2");
    EXPECT_EQ(logs.at(3)->GetAbbreviatedTreeHashID(), L"B2Short");
    EXPECT_EQ(logs.at(3)->GetParentHashIDs().size(), (size_t)0);
    EXPECT_EQ(logs.at(3)->GetAbbreviatedParentHashIDs().size(), (size_t)0);
}

TEST_F(GitServiceTest, ParseGitLog)
{
    std::wstring str = L"";
    str += L"commit e6c413289f2c1fd6fc6377495926ec5e5644de98 (HEAD -> 20240114Git, origin/20240114Git)\n";
    str += L"Author:     David <s1155003185@gmail.com>\n";
    str += L"AuthorDate: Thu Jan 25 22:47:35 2024 +0800\n";
    str += L"Commit:     David <s1155003185@gmail.com>\n";
    str += L"CommitDate: Thu Jan 25 22:47:35 2024 +0800\n";
    str += L"\n";
    str += L"    [GIt] Parse Git Log Graph\n";
    str += L"\n";

    DECLARE_SPTR(GitLog, log1);
    GitService::ParseGitLog(str, log1);
    EXPECT_EQ(log1->GetHashID(), L"e6c413289f2c1fd6fc6377495926ec5e5644de98");
    EXPECT_EQ(log1->GetIsHead(), true);
    EXPECT_EQ(log1->GetTags().size(), (size_t)2);
    EXPECT_EQ(log1->GetTags().at(0), L"20240114Git");
    EXPECT_EQ(log1->GetTags().at(1), L"origin/20240114Git");
    EXPECT_EQ(log1->GetAuthor(), L"David");
    EXPECT_EQ(log1->GetAuthorEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log1->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->GetAuthorDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->GetCommitter(), L"David");
    EXPECT_EQ(log1->GetCommitterEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log1->GetCommitDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->GetCommitDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->GetTitle(), L"[GIt] Parse Git Log Graph");
    EXPECT_EQ(log1->GetMessage(), L"");
    EXPECT_EQ(log1->GetFullMessage(), str);
    
    str = L"commit 660e49f210903438ccc1c959912a995e0b06b3a8\n";
    str += L"Author:     David <s1155003185@gmail.com>\n";
    str += L"AuthorDate: Thu Jan 25 21:30:19 2024 +0800\n";
    str += L"Commit:     David <s1155003185@gmail.com>\n";
    str += L"CommitDate: Thu Jan 25 21:30:19 2024 +0800\n";
    str += L"\n";
    str += L"    [GIt] Git Log Search Criteria\n";
    str += L"\n";
    str += L"    CEF-related documentation and build improvements.\n";
    str += L"\n";
    str += L"    See #24230.\n";
    str += L"\n";
    DECLARE_SPTR(GitLog, log2);
    GitService::ParseGitLog(str, log2);
    EXPECT_EQ(log2->GetHashID(), L"660e49f210903438ccc1c959912a995e0b06b3a8");
    EXPECT_EQ(log2->GetIsHead(), false);
    EXPECT_EQ(log2->GetTags().size(), (size_t)0);
    EXPECT_EQ(log2->GetAuthor(), L"David");
    EXPECT_EQ(log2->GetAuthorEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log2->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->GetAuthorDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->GetCommitter(), L"David");
    EXPECT_EQ(log2->GetCommitterEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log2->GetCommitDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->GetCommitDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->GetTitle(), L"[GIt] Git Log Search Criteria");
    EXPECT_EQ(log2->GetMessage(), L"CEF-related documentation and build improvements.\n\nSee #24230.\n");
    EXPECT_EQ(log2->GetFullMessage(), str);

    str = L"commit d34a8e0f04315ea9f3d906cd6e05fee8af63286c\n";
    str += L"Author:     David <s1155003185@gmail.com>\n";
    str += L"AuthorDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"Commit:     David <s1155003185@gmail.com>\n";
    str += L"CommitDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"\n";
    str += L"    [Git] Log and Branch\n";
    str += L"\n";
    DECLARE_SPTR(GitLog, log3);
    GitService::ParseGitLog(str, log3);
    EXPECT_EQ(log3->GetHashID(), L"d34a8e0f04315ea9f3d906cd6e05fee8af63286c");
    EXPECT_EQ(log3->GetIsHead(), false);
    EXPECT_EQ(log3->GetTags().size(), (size_t)0);
    EXPECT_EQ(log3->GetAuthor(), L"David");
    EXPECT_EQ(log3->GetAuthorEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log3->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->GetAuthorDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->GetCommitter(), L"David");
    EXPECT_EQ(log3->GetCommitterEmail(), L"s1155003185@gmail.com");
    EXPECT_EQ(log3->GetCommitDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->GetCommitDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->GetTitle(), L"[Git] Log and Branch");
    EXPECT_EQ(log3->GetMessage(), L"");
    EXPECT_EQ(log3->GetFullMessage(), str);
}

TEST_F(GitServiceTest, LogAndBranch)
{

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

TEST_F(GitServiceTest, StageAndDifference)
{
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
