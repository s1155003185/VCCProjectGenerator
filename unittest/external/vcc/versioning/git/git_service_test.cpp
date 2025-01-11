#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

#include <iostream>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "log_config.hpp"
#include "terminal_service.hpp"

using namespace vcc;

class GitServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/Git/");
    public:

        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);

            if (IsDirectoryExists(this->GetWorkspace())) {
                #ifdef __WIN32
                    // window git will hold some files in bin/Debug/Git folder, cannot delete whole folder by c++ remove_all 
                    TerminalService::Execute(this->GetLogConfig().get(), L"", L"rmdir /s /q \"" + ConcatPaths({std::filesystem::current_path().wstring(), this->GetWorkspace()}) + L"\"");
                #else
                    std::filesystem::remove_all(this->GetWorkspace());
                #endif
            }
            
            CreateDirectory(this->GetWorkspace());
        }

        void TearDown() override
        {

        }
};

TEST_F(GitServiceTest, Version)
{
    std::wstring version = GitService::GetVersion(this->GetLogConfig().get());
    EXPECT_TRUE(regex_match(version, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")));
}

// Note: Only execute command project Unit test
// Not execute for common project
// TEST_F(GitServiceTest, Remote)
// {
//     std::vector<std::shared_ptr<GitRemote>> remotes;
//     GitService::GetRemote(this->GetLogConfig().get(), L"", remotes);
//     EXPECT_EQ(remotes.size(), (size_t)2);
//     EXPECT_EQ(remotes.at(0)->GetName(), L"origin");
//     EXPECT_EQ(remotes.at(0)->GetURL(), L"https://github.com/s1155003185/VCCProjectGenerator.git");
//     EXPECT_EQ(remotes.at(0)->GetMirror(), GitRemoteMirror::Fetch);
//     EXPECT_EQ(remotes.at(1)->GetName(), L"origin");
//     EXPECT_EQ(remotes.at(1)->GetURL(), L"https://github.com/s1155003185/VCCProjectGenerator.git");
//     EXPECT_EQ(remotes.at(1)->GetMirror(), GitRemoteMirror::Push);
// }

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
    EXPECT_EQ(logs.at(0)->GetColumnIndex(), 0);
    EXPECT_EQ(logs.at(0)->GetHashID(), L"A1");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedHashID(), L"A1Short");
    EXPECT_EQ(logs.at(0)->GetTreeHashID(), L"A2");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedTreeHashID(), L"A2Short");
    EXPECT_EQ(logs.at(0)->GetParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->GetParentHashIDs().at(0), L"A3");
    EXPECT_EQ(logs.at(0)->GetAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->GetAbbreviatedParentHashIDs().at(0), L"A3Short");

    EXPECT_EQ(logs.at(1)->GetColumnIndex(), 0);
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

    EXPECT_EQ(logs.at(2)->GetColumnIndex(), 1);
    EXPECT_EQ(logs.at(2)->GetHashID(), L"A6");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedHashID(), L"A6Short");
    EXPECT_EQ(logs.at(2)->GetTreeHashID(), L"A7");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedTreeHashID(), L"A7Short");
    EXPECT_EQ(logs.at(2)->GetParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->GetParentHashIDs().at(0), L"A8");
    EXPECT_EQ(logs.at(2)->GetAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->GetAbbreviatedParentHashIDs().at(0), L"A8Short");

    EXPECT_EQ(logs.at(3)->GetColumnIndex(), 0);
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
    // One Commit only 
    str = L"commit 8779347dd3e26367ba47327b47343c8dc30bd18c (HEAD -> main)\n";
    str += L"Author: Test Tester <test@gmail.com>\n";
    str += L"AuthorDate:   Tue Feb 13 18:11:56 2024 +0800\n";
    str += L"\n";
    str += L"    Test Commit\n";

    auto log0 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log0);
    EXPECT_EQ(log0->GetHashID(), L"8779347dd3e26367ba47327b47343c8dc30bd18c");
    EXPECT_EQ(log0->GetIsHead(), true);
    EXPECT_EQ(log0->GetBranches().size(), (size_t)1);
    EXPECT_EQ(log0->GetBranches().at(0), L"main");
    EXPECT_EQ(log0->GetTags().size(), (size_t)0);
    EXPECT_EQ(log0->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log0->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log0->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Tue Feb 13 18:11:56 2024 +0800"));
    EXPECT_EQ(log0->GetAuthorDateStr(), L"Tue Feb 13 18:11:56 2024 +0800");
    EXPECT_EQ(log0->GetCommitter(), L"");
    EXPECT_EQ(log0->GetCommitterEmail(), L"");
    EXPECT_EQ(log0->GetCommitDate(), -1);
    EXPECT_EQ(log0->GetCommitDateStr(), L"");
    EXPECT_EQ(log0->GetTitle(), L"Test Commit");
    EXPECT_EQ(log0->GetMessage(), L"");
    EXPECT_EQ(log0->GetFullMessage(), str);

    // branch
    str = L"commit e6c413289f2c1fd6fc6377495926ec5e5644de98 (HEAD -> 20240114Git, origin/20240114Git)\n";
    str += L"Author:     Test Tester <test@gmail.com>\n";
    str += L"AuthorDate: Thu Jan 25 22:47:35 2024 +0800\n";
    str += L"Commit:     Test Tester <test@gmail.com>\n";
    str += L"CommitDate: Thu Jan 25 22:47:35 2024 +0800\n";
    str += L"\n";
    str += L"    [GIt] Parse Git Log Graph\n";
    str += L"\n";

    auto log1 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log1);
    EXPECT_EQ(log1->GetHashID(), L"e6c413289f2c1fd6fc6377495926ec5e5644de98");
    EXPECT_EQ(log1->GetIsHead(), true);
    EXPECT_EQ(log1->GetBranches().size(), (size_t)2);
    EXPECT_EQ(log1->GetBranches().at(0), L"20240114Git");
    EXPECT_EQ(log1->GetBranches().at(1), L"origin/20240114Git");
    EXPECT_EQ(log1->GetTags().size(), (size_t)0);
    EXPECT_EQ(log1->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log1->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log1->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->GetAuthorDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->GetCommitter(), L"Test Tester");
    EXPECT_EQ(log1->GetCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log1->GetCommitDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->GetCommitDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->GetTitle(), L"[GIt] Parse Git Log Graph");
    EXPECT_EQ(log1->GetMessage(), L"");
    EXPECT_EQ(log1->GetFullMessage(), str);
    
    // double command
    str = L"commit 660e49f210903438ccc1c959912a995e0b06b3a8\n";
    str += L"Author:     Test Tester <test@gmail.com>\n";
    str += L"AuthorDate: Thu Jan 25 21:30:19 2024 +0800\n";
    str += L"Commit:     Test Tester <test@gmail.com>\n";
    str += L"CommitDate: Thu Jan 25 21:30:19 2024 +0800\n";
    str += L"\n";
    str += L"    [GIt] Git Log Search Criteria\n";
    str += L"\n";
    str += L"    CEF-related documentation and build improvements.\n";
    str += L"\n";
    str += L"    See #24230.\n";
    str += L"\n";
    auto log2 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log2);
    EXPECT_EQ(log2->GetHashID(), L"660e49f210903438ccc1c959912a995e0b06b3a8");
    EXPECT_EQ(log2->GetIsHead(), false);
    EXPECT_EQ(log2->GetBranches().size(), (size_t)0);
    EXPECT_EQ(log2->GetTags().size(), (size_t)0);
    EXPECT_EQ(log2->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log2->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log2->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->GetAuthorDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->GetCommitter(), L"Test Tester");
    EXPECT_EQ(log2->GetCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log2->GetCommitDate(), GitService::ParseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->GetCommitDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->GetTitle(), L"[GIt] Git Log Search Criteria");
    EXPECT_EQ(log2->GetMessage(), L"CEF-related documentation and build improvements.\n\nSee #24230.\n");
    EXPECT_EQ(log2->GetFullMessage(), str);

    // normal
    str = L"commit d34a8e0f04315ea9f3d906cd6e05fee8af63286c\n";
    str += L"Author:     Test Tester <test@gmail.com>\n";
    str += L"AuthorDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"Commit:     Test Tester <test@gmail.com>\n";
    str += L"CommitDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"\n";
    str += L"    [Git] Log and Branch\n";
    str += L"\n";
    auto log3 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log3);
    EXPECT_EQ(log3->GetHashID(), L"d34a8e0f04315ea9f3d906cd6e05fee8af63286c");
    EXPECT_EQ(log3->GetIsHead(), false);
    EXPECT_EQ(log3->GetBranches().size(), (size_t)0);
    EXPECT_EQ(log3->GetTags().size(), (size_t)0);
    EXPECT_EQ(log3->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log3->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log3->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->GetAuthorDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->GetCommitter(), L"Test Tester");
    EXPECT_EQ(log3->GetCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log3->GetCommitDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->GetCommitDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->GetTitle(), L"[Git] Log and Branch");
    EXPECT_EQ(log3->GetMessage(), L"");
    EXPECT_EQ(log3->GetFullMessage(), str);

    // tag only
    str = L"commit d34a8e0f04315ea9f3d906cd6e05fee8af63286c (tag: v1.0.12)\n";
    str += L"Author:     Test Tester <test@gmail.com>\n";
    str += L"AuthorDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"Commit:     Test Tester <test@gmail.com>\n";
    str += L"CommitDate: Mon Jan 22 20:41:42 2024 +0800\n";
    str += L"\n";
    str += L"    [Git] Log and Branch\n";
    str += L"\n";
    auto log4 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log4);
    EXPECT_EQ(log4->GetHashID(), L"d34a8e0f04315ea9f3d906cd6e05fee8af63286c");
    EXPECT_EQ(log4->GetIsHead(), false);
    EXPECT_EQ(log4->GetBranches().size(), (size_t)0);
    EXPECT_EQ(log4->GetTags().size(), (size_t)1);
    EXPECT_EQ(log4->GetTags().at(0), L"v1.0.12");
    EXPECT_EQ(log4->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log4->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log4->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log4->GetAuthorDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log4->GetCommitter(), L"Test Tester");
    EXPECT_EQ(log4->GetCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log4->GetCommitDate(), GitService::ParseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log4->GetCommitDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log4->GetTitle(), L"[Git] Log and Branch");
    EXPECT_EQ(log4->GetMessage(), L"");
    EXPECT_EQ(log4->GetFullMessage(), str);

    // mixed
    str = L"commit a4da73bfdd353931c0146138d931f17c533e561a (HEAD -> main, tag: v0.0.2, tag: v0.0.1, branch2, branch)\n";
    str += L"Author: Test Tester <test@gmail.com>\n";
    str += L"AuthorDate:   Tue Feb 13 18:07:12 2024 +0800\n";
    str += L"\n";
    str += L"    Test Commit\n";
    auto log5 = std::make_shared<GitLog>();
    GitService::ParseGitLog(str, log5);
    EXPECT_EQ(log5->GetHashID(), L"a4da73bfdd353931c0146138d931f17c533e561a");
    EXPECT_EQ(log5->GetIsHead(), true);
    EXPECT_EQ(log5->GetBranches().size(), (size_t)3);
    EXPECT_EQ(log5->GetBranches().at(0), L"main");
    EXPECT_EQ(log5->GetBranches().at(1), L"branch2");
    EXPECT_EQ(log5->GetBranches().at(2), L"branch");
    EXPECT_EQ(log5->GetTags().size(), (size_t)2);
    EXPECT_EQ(log5->GetTags().at(0), L"v0.0.2");
    EXPECT_EQ(log5->GetTags().at(1), L"v0.0.1");
    EXPECT_EQ(log5->GetAuthor(), L"Test Tester");
    EXPECT_EQ(log5->GetAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log5->GetAuthorDate(), GitService::ParseGitLogDatetime(L"Tue Feb 13 18:07:12 2024 +0800"));
    EXPECT_EQ(log5->GetAuthorDateStr(), L"Tue Feb 13 18:07:12 2024 +0800");
    EXPECT_EQ(log5->GetCommitter(), L"");
    EXPECT_EQ(log5->GetCommitterEmail(), L"");
    EXPECT_EQ(log5->GetCommitDate(), -1);
    EXPECT_EQ(log5->GetCommitDateStr(), L"");
    EXPECT_EQ(log5->GetTitle(), L"Test Commit");
    EXPECT_EQ(log5->GetMessage(), L"");
    EXPECT_EQ(log5->GetFullMessage(), str);
}

TEST_F(GitServiceTest, ParseGitBranch)
{
    std::wstring str = L"* master hashID Title 1";
    auto checkoutBranch1 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch1);
    EXPECT_EQ(checkoutBranch1->GetName(), L"master");
    EXPECT_EQ(checkoutBranch1->GetIsActive(), true);
    EXPECT_EQ(checkoutBranch1->GetHashID(), L"hashID");
    EXPECT_EQ(checkoutBranch1->GetTitle(), L"Title 1");

    str = L" master hashID Title 1";
    auto checkoutBranch2 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch2);
    EXPECT_EQ(checkoutBranch2->GetName(), L"master");
    EXPECT_EQ(checkoutBranch2->GetIsActive(), false);
    EXPECT_EQ(checkoutBranch2->GetHashID(), L"hashID");
    EXPECT_EQ(checkoutBranch2->GetTitle(), L"Title 1");

    str = L" master -> origin/Head";
    auto checkoutBranch3 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch3);
    EXPECT_EQ(checkoutBranch3->GetName(), L"master");
    EXPECT_EQ(checkoutBranch3->GetIsActive(), false);
    EXPECT_EQ(checkoutBranch3->GetHashID(), L"");
    EXPECT_EQ(checkoutBranch3->GetTitle(), L"");
    EXPECT_EQ(checkoutBranch3->GetPointToBranch(), L"origin/Head");
}

TEST_F(GitServiceTest, Tag)
{
    // init
    GitService::Initialize(this->GetLogConfig().get(), this->GetWorkspace());

    WriteFile(ConcatPaths({this->GetWorkspace(), L"test.txt"}), L"hi\r\n", true);
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Commit");
    // meaningless to test exception case
    // try
    // {
    //     GitTagCurrentTag noTag = GitService::GetCurrentTag(this->GetLogConfig().get(), this->GetWorkspace());
    //     // show throw exception
    //     EXPECT_TRUE(false);
    // }
    // catch(...)
    // {
    //     EXPECT_TRUE(true);
    // }
    
    GitService::CreateTag(this->GetLogConfig().get(), this->GetWorkspace(), L"v0.0.1", nullptr);
    std::vector<std::wstring> tags;
    GitService::GetTags(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, tags);
    EXPECT_EQ(tags.size(), (size_t)1);
    EXPECT_EQ(tags[0], L"v0.0.1");
    GitTagCurrentTag currentTag = GitService::GetCurrentTag(this->GetLogConfig().get(), this->GetWorkspace());
    EXPECT_EQ(currentTag.GetTagName(), L"v0.0.1");
    EXPECT_EQ(currentTag.GetNoOfCommit(), 0);

    // Window behavior and Linux Behavior different, Window throw exception (tag will detach branch) while Linux will not
    // Can use GitService::SwitchReverse to switch back
    // As window will throw exception, meaningless to have unit test for switching tag
    // try {
    //     GitService::Switch(this->GetLogConfig().get(), this->GetWorkspace(), L"v0.0.1");
    // } catch (...) {

    // }
    //GitService::SwitchReverse(this->GetLogConfig().get(), this->GetWorkspace());

    // DECLARE_SPTR(GitLog, log);
    // GitService::GetTag(this->GetLogConfig().get(), this->GetWorkspace(), L"v0.0.1", log);
    // EXPECT_EQ(log->GetTags().size(), (size_t)1);
    // EXPECT_EQ(log->GetTags().at(0), L"v0.0.1");

    GitService::DeleteTag(this->GetLogConfig().get(), this->GetWorkspace(), L"v0.0.1");
}

TEST_F(GitServiceTest, Branch)
{
    // init
    GitService::Initialize(this->GetLogConfig().get(), this->GetWorkspace());

    WriteFile(ConcatPaths({this->GetWorkspace(), L"test.txt"}), L"hi\r\n", true);
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Commit");
   
    // Create Branch
    GitService::CreateBranch(this->GetLogConfig().get(), this->GetWorkspace(), L"branch", nullptr);
    // main for linux, master for window
    EXPECT_TRUE(GitService::GetCurrentBranchName(this->GetLogConfig().get(), this->GetWorkspace()) == L"main"
        || GitService::GetCurrentBranchName(this->GetLogConfig().get(), this->GetWorkspace()) == L"master");

    // Current Branch
    // DECLARE_SPTR(GitBranch, currentbranch);
    // GitService::GetCurrentBranch(this->GetLogConfig().get(), this->GetWorkspace(), currentbranch);
    // EXPECT_EQ(currentbranch->GetName(), L"branch");
    // EXPECT_EQ(currentbranch->GetIsActive(), false);
    // EXPECT_TRUE(!currentbranch->GetHashID().empty());
    // EXPECT_TRUE(!currentbranch->GetTitle().empty());
    // EXPECT_EQ(currentbranch->GetPointToBranch(), L""); 

    // Switch Branch
    GitBranchSwitchBranchOption switchBranchOption;
    switchBranchOption.SetIsQuite(true);
    GitService::SwitchBranch(this->GetLogConfig().get(), this->GetWorkspace(), L"branch", &switchBranchOption);
    // DECLARE_SPTR(GitBranch, switchBranch);
    // GitService::GetCurrentBranch(this->GetLogConfig().get(), this->GetWorkspace(), switchBranch);
    // EXPECT_EQ(switchBranch->GetName(), L"branch");
    // EXPECT_EQ(switchBranch->GetIsActive(), true);
    // EXPECT_TRUE(!switchBranch->GetHashID().empty());
    // EXPECT_TRUE(!switchBranch->GetTitle().empty());
    // EXPECT_EQ(switchBranch->GetPointToBranch(), L""); 
    EXPECT_EQ(GitService::GetCurrentBranchName(this->GetLogConfig().get(), this->GetWorkspace()), L"branch");

    // Get Branches
    std::vector<std::shared_ptr<GitBranch>> branches;
    GitService::GetBranches(this->GetLogConfig().get(), this->GetWorkspace(), branches);
    EXPECT_EQ(branches.at(0)->GetName(), L"branch");
    EXPECT_TRUE(branches.at(1)->GetName() == L"main" || branches.at(1)->GetName() == L"master");

    // Delete
    // no testing switch as window use master but linux use main
    //GitService::SwitchBranch(this->GetLogConfig().get(), this->GetWorkspace(), L"main");
    // cannot delete as still in main branch
    //GitService::DeleteBranch(this->GetLogConfig().get(), this->GetWorkspace(), L"branch");
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
    auto diff = std::make_shared<GitDifference>();
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
    GitService::Initialize(this->GetLogConfig().get(), this->GetWorkspace());
    EXPECT_TRUE(std::filesystem::exists(this->GetWorkspace() + L"/.git/HEAD"));
    
    // check existance
    EXPECT_TRUE(GitService::IsGitResponse(this->GetLogConfig().get(), this->GetWorkspace()));

    // config
    auto config = std::make_shared<GitConfig>();
    GitService::GetLocalConfig(this->GetLogConfig().get(), this->GetWorkspace(), config);
    EXPECT_TRUE(config->GetUserName().empty());
    EXPECT_TRUE(config->GetUserEmail().empty());
    
    GitService::SetLocalUserName(this->GetLogConfig().get(), this->GetWorkspace(), L"test");
    GitService::SetLocalUserEmail(this->GetLogConfig().get(), this->GetWorkspace(), L"test@test.com");
    EXPECT_EQ(GitService::GetLocalUserName(this->GetLogConfig().get(), this->GetWorkspace()), L"test");
    EXPECT_EQ(GitService::GetLocalUserEmail(this->GetLogConfig().get(), this->GetWorkspace()), L"test@test.com");
    EXPECT_EQ(GitService::GetUserName(this->GetLogConfig().get(), this->GetWorkspace()), L"test");
    EXPECT_EQ(GitService::GetUserEmail(this->GetLogConfig().get(), this->GetWorkspace()), L"test@test.com");
    EXPECT_TRUE(GitService::IsLocalConfigExists(this->GetLogConfig().get(), this->GetWorkspace(), L"user.name"));

    // Case: Empty File
    auto statusEmpty = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusEmpty);
    // main for linux, master for window
    EXPECT_TRUE(statusEmpty->GetBranch() == L"main" || statusEmpty->GetBranch() == L"master");
    EXPECT_EQ(statusEmpty->GetRemoteBranch(), L"");

    // Case: New File
    WriteFile(ConcatPaths({this->GetWorkspace(), L"test.txt"}), L"hi\r\n", true);
    auto statusCreateFile = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusCreateFile);
    EXPECT_EQ(statusCreateFile->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test.txt");

    // Case: Staged
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    auto statusNewState = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusNewState);
    EXPECT_EQ(statusNewState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusNewState->GetIndexFiles()[GitFileStatus::Added].size(), (size_t)1);
    EXPECT_EQ(statusNewState->GetIndexFiles()[GitFileStatus::Added].at(0), L"test.txt");
    EXPECT_EQ(statusNewState->GetWorkingTreeFiles().size(), (size_t)0);
    GitService::Unstage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    GitService::StageAll(this->GetLogConfig().get(), this->GetWorkspace());
    GitService::UnstageAll(this->GetLogConfig().get(), this->GetWorkspace());
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
 
    // Case: Committed
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Commit");
    auto statusNewCommit = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusNewCommit);
    EXPECT_EQ(statusNewCommit->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusNewCommit->GetWorkingTreeFiles().size(), (size_t)0);
    
    // Case: Modified
    AppendFileOneLine(ConcatPaths({this->GetWorkspace(), L"test.txt"}), L"HI", false);
    auto statusModify = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusModify);
    EXPECT_EQ(statusModify->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModify->GetWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    auto differentSummary = std::make_shared<GitDifferenceSummary>();
    GitService::GetDifferenceSummary(this->GetLogConfig().get(), this->GetWorkspace(), {}, differentSummary);
    EXPECT_EQ(differentSummary->GetFiles().size(), (size_t)1);
    EXPECT_EQ(differentSummary->GetFiles().at(0), L"test.txt");
    EXPECT_EQ(differentSummary->GetAddLineCounts().at(0), (size_t)1);
    EXPECT_EQ(differentSummary->GetDeleteLineCounts().at(0), (size_t)0);
    auto diff = std::make_shared<GitDifference>();
    GitService::GetDifferenceWorkingFile(this->GetLogConfig().get(), this->GetWorkspace(), {}, L"test.txt", diff); // output in ParseGitDiff
    EXPECT_EQ(diff->GetFilePathOld(), L"test.txt");
    EXPECT_EQ(diff->GetFilePathNew(), L"test.txt");
    EXPECT_EQ(diff->GetLineNumberOld()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountOld()[0], (size_t)0);
    EXPECT_EQ(diff->GetLineNumberNew()[0], (size_t)1);
    EXPECT_EQ(diff->GetLineCountNew()[0], (size_t)2);
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    auto statusModifyState = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusModifyState);
    EXPECT_EQ(statusModifyState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState->GetIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState->GetIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState->GetWorkingTreeFiles().size(), (size_t)0);
    AppendFileOneLine(ConcatPaths({this->GetWorkspace(), L"test.txt"}), L"BI", false);
    auto statusModifyState2 = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusModifyState2);
    EXPECT_EQ(statusModifyState2->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->GetWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");

    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test.txt");
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Modify");

    // Case: Renamed
    std::filesystem::rename(ConcatPaths({this->GetWorkspace(), L"test.txt"}), ConcatPaths({this->GetWorkspace(), L"test2.txt"}));
    auto statusRename = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusRename);
    EXPECT_EQ(statusRename->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles().size(), (size_t)2);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test2.txt");
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusRename->GetWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test.txt");
    GitService::StageAll(this->GetLogConfig().get(), this->GetWorkspace());
    auto statusRenameState = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusRenameState);
    EXPECT_EQ(statusRenameState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusRenameState->GetIndexFiles()[GitFileStatus::Renamed].size(), (size_t)1);
    EXPECT_EQ(statusRenameState->GetIndexFiles()[GitFileStatus::Renamed].at(0), L"test.txt -> test2.txt");
    EXPECT_EQ(statusRenameState->GetWorkingTreeFiles().size(), (size_t)0);
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Rename");

    // Case: Deleted
    RemoveFile(ConcatPaths({this->GetWorkspace(), L"test2.txt"}));
    auto statusDelete = std::make_shared<GitStatus>();
    GitService::GetStatus(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, statusDelete);
    EXPECT_EQ(statusDelete->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusDelete->GetWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test2.txt");
    GitService::Stage(this->GetLogConfig().get(), this->GetWorkspace(), L"test2.txt");
    GitService::Commit(this->GetLogConfig().get(), this->GetWorkspace(), L"Test Delete");

    // Summary Log
    std::vector<std::shared_ptr<GitLog>> logs;
    GitService::GetLogs(this->GetLogConfig().get(), this->GetWorkspace(), nullptr, logs);
    EXPECT_EQ(logs.size(), (size_t)4);
    EXPECT_EQ(logs.at(0)->GetTitle(), L"Test Delete");
    EXPECT_EQ(logs.at(1)->GetTitle(), L"Test Rename");
    EXPECT_EQ(logs.at(2)->GetTitle(), L"Test Modify");
    EXPECT_EQ(logs.at(3)->GetTitle(), L"Test Commit");

    // Test normal operation
    GitService::FetchAll(this->GetLogConfig().get(), this->GetWorkspace());
    // Note: cannot pull as it is local response
    //GitService::Pull(this->GetLogConfig().get(), this->GetWorkspace(), nullptr);
}

// TEST_F(GitServiceTest, Config)
// {
//     DECLARE_SPTR(GitConfig, config);
//     GitService::GetGlobalConfig(this->GetLogConfig().get(), config);
//     std::wstring userName = GitService::GetGlobalUserName(this->GetLogConfig().get());
//     std::wstring userEmail = GitService::GetGlobalUserEmail(this->GetLogConfig().get());
//     EXPECT_TRUE(!config->GetUserName().empty());
//     EXPECT_TRUE(!config->GetUserEmail().empty());
//     EXPECT_EQ(config->GetUserName(), userName);
//     EXPECT_EQ(config->GetUserEmail(), userEmail);
// }
