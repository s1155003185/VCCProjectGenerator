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
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/Git/");
    public:

        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);

            if (isDirectoryExists(this->getWorkspace())) {
                #ifdef __WIN32
                    // window git will hold some files in bin/Debug/Git folder, cannot delete whole folder by c++ remove_all 
                    TerminalService::execute(this->getLogConfig().get(), L"", L"rmdir /s /q \"" + concatPaths({std::filesystem::current_path().wstring(), this->getWorkspace()}) + L"\"");
                #else
                    std::filesystem::remove_all(this->getWorkspace());
                #endif
            }
            
            createDirectory(this->getWorkspace());
        }

        void TearDown() override
        {

        }
};

TEST_F(GitServiceTest, Version)
{
    std::wstring version = GitService::getVersion(this->getLogConfig().get());
    EXPECT_TRUE(regex_match(version, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")));
}

// Note: Only execute command project Unit test
// Not execute for common project
// TEST_F(GitServiceTest, Remote)
// {
//     std::vector<std::shared_ptr<GitRemote>> remotes;
//     GitService::getRemote(this->getLogConfig().get(), L"", remotes);
//     EXPECT_EQ(remotes.size(), (size_t)2);
//     EXPECT_EQ(remotes.at(0)->getName(), L"origin");
//     EXPECT_EQ(remotes.at(0)->getURL(), L"https://github.com/s1155003185/VCCProjectGenerator.git");
//     EXPECT_EQ(remotes.at(0)->getMirror(), GitRemoteMirror::Fetch);
//     EXPECT_EQ(remotes.at(1)->getName(), L"origin");
//     EXPECT_EQ(remotes.at(1)->getURL(), L"https://github.com/s1155003185/VCCProjectGenerator.git");
//     EXPECT_EQ(remotes.at(1)->getMirror(), GitRemoteMirror::Push);
// }

TEST_F(GitServiceTest, parseGitLogGraph)
{
    std::wstring str = L"";
    str += L"* (A1)(A1Short)(A2)(A2Short)(A3)(A3Short)\n"; // normal
    str += L"*   (A3)(A3Short)(A4)(A4Short)(A5 A6)(A5Short A6Short)\n"; // merge
    str += L"|\n";
    str += L"| * (A6)(A6Short)(A7)(A7Short)(A8)(A8Short)\r\n"; // merge branch
    str += L"* (B1)(B1Short)(B2)(B2Short)()()\r\n"; // node init

    std::vector<std::shared_ptr<GitLog>> logs = GitService::parseGitLogGraph(str);
    EXPECT_EQ(logs.size(), (size_t)4);
    EXPECT_EQ(logs.at(0)->getColumnIndex(), 0);
    EXPECT_EQ(logs.at(0)->getHashID(), L"A1");
    EXPECT_EQ(logs.at(0)->getAbbreviatedHashID(), L"A1Short");
    EXPECT_EQ(logs.at(0)->getTreeHashID(), L"A2");
    EXPECT_EQ(logs.at(0)->getAbbreviatedTreeHashID(), L"A2Short");
    EXPECT_EQ(logs.at(0)->getParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->getParentHashIDs().at(0), L"A3");
    EXPECT_EQ(logs.at(0)->getAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(0)->getAbbreviatedParentHashIDs().at(0), L"A3Short");

    EXPECT_EQ(logs.at(1)->getColumnIndex(), 0);
    EXPECT_EQ(logs.at(1)->getHashID(), L"A3");
    EXPECT_EQ(logs.at(1)->getAbbreviatedHashID(), L"A3Short");
    EXPECT_EQ(logs.at(1)->getTreeHashID(), L"A4");
    EXPECT_EQ(logs.at(1)->getAbbreviatedTreeHashID(), L"A4Short");
    EXPECT_EQ(logs.at(1)->getParentHashIDs().size(), (size_t)2);
    EXPECT_EQ(logs.at(1)->getParentHashIDs().at(0), L"A5");
    EXPECT_EQ(logs.at(1)->getParentHashIDs().at(1), L"A6");
    EXPECT_EQ(logs.at(1)->getAbbreviatedParentHashIDs().size(), (size_t)2);
    EXPECT_EQ(logs.at(1)->getAbbreviatedParentHashIDs().at(0), L"A5Short");
    EXPECT_EQ(logs.at(1)->getAbbreviatedParentHashIDs().at(1), L"A6Short");

    EXPECT_EQ(logs.at(2)->getColumnIndex(), 1);
    EXPECT_EQ(logs.at(2)->getHashID(), L"A6");
    EXPECT_EQ(logs.at(2)->getAbbreviatedHashID(), L"A6Short");
    EXPECT_EQ(logs.at(2)->getTreeHashID(), L"A7");
    EXPECT_EQ(logs.at(2)->getAbbreviatedTreeHashID(), L"A7Short");
    EXPECT_EQ(logs.at(2)->getParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->getParentHashIDs().at(0), L"A8");
    EXPECT_EQ(logs.at(2)->getAbbreviatedParentHashIDs().size(), (size_t)1);
    EXPECT_EQ(logs.at(2)->getAbbreviatedParentHashIDs().at(0), L"A8Short");

    EXPECT_EQ(logs.at(3)->getColumnIndex(), 0);
    EXPECT_EQ(logs.at(3)->getHashID(), L"B1");
    EXPECT_EQ(logs.at(3)->getAbbreviatedHashID(), L"B1Short");
    EXPECT_EQ(logs.at(3)->getTreeHashID(), L"B2");
    EXPECT_EQ(logs.at(3)->getAbbreviatedTreeHashID(), L"B2Short");
    EXPECT_EQ(logs.at(3)->getParentHashIDs().size(), (size_t)0);
    EXPECT_EQ(logs.at(3)->getAbbreviatedParentHashIDs().size(), (size_t)0);
}

TEST_F(GitServiceTest, parseGitLog)
{
    std::wstring str = L"";
    // One Commit only 
    str = L"commit 8779347dd3e26367ba47327b47343c8dc30bd18c (HEAD -> main)\n";
    str += L"Author: Test Tester <test@gmail.com>\n";
    str += L"AuthorDate:   Tue Feb 13 18:11:56 2024 +0800\n";
    str += L"\n";
    str += L"    Test Commit\n";

    auto log0 = std::make_shared<GitLog>();
    GitService::parseGitLog(str, log0);
    EXPECT_EQ(log0->getHashID(), L"8779347dd3e26367ba47327b47343c8dc30bd18c");
    EXPECT_EQ(log0->getIsHead(), true);
    EXPECT_EQ(log0->getBranches().size(), (size_t)1);
    EXPECT_EQ(log0->getBranches().at(0), L"main");
    EXPECT_EQ(log0->getTags().size(), (size_t)0);
    EXPECT_EQ(log0->getAuthor(), L"Test Tester");
    EXPECT_EQ(log0->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log0->getAuthorDate(), GitService::parseGitLogDatetime(L"Tue Feb 13 18:11:56 2024 +0800"));
    EXPECT_EQ(log0->getAuthorDateStr(), L"Tue Feb 13 18:11:56 2024 +0800");
    EXPECT_EQ(log0->getCommitter(), L"");
    EXPECT_EQ(log0->getCommitterEmail(), L"");
    EXPECT_EQ(log0->getCommitDate(), -1);
    EXPECT_EQ(log0->getCommitDateStr(), L"");
    EXPECT_EQ(log0->getTitle(), L"Test Commit");
    EXPECT_EQ(log0->getMessage(), L"");
    EXPECT_EQ(log0->getFullMessage(), str);

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
    GitService::parseGitLog(str, log1);
    EXPECT_EQ(log1->getHashID(), L"e6c413289f2c1fd6fc6377495926ec5e5644de98");
    EXPECT_EQ(log1->getIsHead(), true);
    EXPECT_EQ(log1->getBranches().size(), (size_t)2);
    EXPECT_EQ(log1->getBranches().at(0), L"20240114Git");
    EXPECT_EQ(log1->getBranches().at(1), L"origin/20240114Git");
    EXPECT_EQ(log1->getTags().size(), (size_t)0);
    EXPECT_EQ(log1->getAuthor(), L"Test Tester");
    EXPECT_EQ(log1->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log1->getAuthorDate(), GitService::parseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->getAuthorDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->getCommitter(), L"Test Tester");
    EXPECT_EQ(log1->getCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log1->getCommitDate(), GitService::parseGitLogDatetime(L"Thu Jan 25 22:47:35 2024 +0800"));
    EXPECT_EQ(log1->getCommitDateStr(), L"Thu Jan 25 22:47:35 2024 +0800");
    EXPECT_EQ(log1->getTitle(), L"[GIt] Parse Git Log Graph");
    EXPECT_EQ(log1->getMessage(), L"");
    EXPECT_EQ(log1->getFullMessage(), str);
    
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
    GitService::parseGitLog(str, log2);
    EXPECT_EQ(log2->getHashID(), L"660e49f210903438ccc1c959912a995e0b06b3a8");
    EXPECT_EQ(log2->getIsHead(), false);
    EXPECT_EQ(log2->getBranches().size(), (size_t)0);
    EXPECT_EQ(log2->getTags().size(), (size_t)0);
    EXPECT_EQ(log2->getAuthor(), L"Test Tester");
    EXPECT_EQ(log2->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log2->getAuthorDate(), GitService::parseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->getAuthorDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->getCommitter(), L"Test Tester");
    EXPECT_EQ(log2->getCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log2->getCommitDate(), GitService::parseGitLogDatetime(L"Thu Jan 25 21:30:19 2024 +0800"));
    EXPECT_EQ(log2->getCommitDateStr(), L"Thu Jan 25 21:30:19 2024 +0800");
    EXPECT_EQ(log2->getTitle(), L"[GIt] Git Log Search Criteria");
    EXPECT_EQ(log2->getMessage(), L"CEF-related documentation and build improvements.\n\nSee #24230.\n");
    EXPECT_EQ(log2->getFullMessage(), str);

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
    GitService::parseGitLog(str, log3);
    EXPECT_EQ(log3->getHashID(), L"d34a8e0f04315ea9f3d906cd6e05fee8af63286c");
    EXPECT_EQ(log3->getIsHead(), false);
    EXPECT_EQ(log3->getBranches().size(), (size_t)0);
    EXPECT_EQ(log3->getTags().size(), (size_t)0);
    EXPECT_EQ(log3->getAuthor(), L"Test Tester");
    EXPECT_EQ(log3->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log3->getAuthorDate(), GitService::parseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->getAuthorDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->getCommitter(), L"Test Tester");
    EXPECT_EQ(log3->getCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log3->getCommitDate(), GitService::parseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log3->getCommitDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log3->getTitle(), L"[Git] Log and Branch");
    EXPECT_EQ(log3->getMessage(), L"");
    EXPECT_EQ(log3->getFullMessage(), str);

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
    GitService::parseGitLog(str, log4);
    EXPECT_EQ(log4->getHashID(), L"d34a8e0f04315ea9f3d906cd6e05fee8af63286c");
    EXPECT_EQ(log4->getIsHead(), false);
    EXPECT_EQ(log4->getBranches().size(), (size_t)0);
    EXPECT_EQ(log4->getTags().size(), (size_t)1);
    EXPECT_EQ(log4->getTags().at(0), L"v1.0.12");
    EXPECT_EQ(log4->getAuthor(), L"Test Tester");
    EXPECT_EQ(log4->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log4->getAuthorDate(), GitService::parseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log4->getAuthorDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log4->getCommitter(), L"Test Tester");
    EXPECT_EQ(log4->getCommitterEmail(), L"test@gmail.com");
    EXPECT_EQ(log4->getCommitDate(), GitService::parseGitLogDatetime(L"Mon Jan 22 20:41:42 2024 +0800"));
    EXPECT_EQ(log4->getCommitDateStr(), L"Mon Jan 22 20:41:42 2024 +0800");
    EXPECT_EQ(log4->getTitle(), L"[Git] Log and Branch");
    EXPECT_EQ(log4->getMessage(), L"");
    EXPECT_EQ(log4->getFullMessage(), str);

    // mixed
    str = L"commit a4da73bfdd353931c0146138d931f17c533e561a (HEAD -> main, tag: v0.0.2, tag: v0.0.1, branch2, branch)\n";
    str += L"Author: Test Tester <test@gmail.com>\n";
    str += L"AuthorDate:   Tue Feb 13 18:07:12 2024 +0800\n";
    str += L"\n";
    str += L"    Test Commit\n";
    auto log5 = std::make_shared<GitLog>();
    GitService::parseGitLog(str, log5);
    EXPECT_EQ(log5->getHashID(), L"a4da73bfdd353931c0146138d931f17c533e561a");
    EXPECT_EQ(log5->getIsHead(), true);
    EXPECT_EQ(log5->getBranches().size(), (size_t)3);
    EXPECT_EQ(log5->getBranches().at(0), L"main");
    EXPECT_EQ(log5->getBranches().at(1), L"branch2");
    EXPECT_EQ(log5->getBranches().at(2), L"branch");
    EXPECT_EQ(log5->getTags().size(), (size_t)2);
    EXPECT_EQ(log5->getTags().at(0), L"v0.0.2");
    EXPECT_EQ(log5->getTags().at(1), L"v0.0.1");
    EXPECT_EQ(log5->getAuthor(), L"Test Tester");
    EXPECT_EQ(log5->getAuthorEmail(), L"test@gmail.com");
    EXPECT_EQ(log5->getAuthorDate(), GitService::parseGitLogDatetime(L"Tue Feb 13 18:07:12 2024 +0800"));
    EXPECT_EQ(log5->getAuthorDateStr(), L"Tue Feb 13 18:07:12 2024 +0800");
    EXPECT_EQ(log5->getCommitter(), L"");
    EXPECT_EQ(log5->getCommitterEmail(), L"");
    EXPECT_EQ(log5->getCommitDate(), -1);
    EXPECT_EQ(log5->getCommitDateStr(), L"");
    EXPECT_EQ(log5->getTitle(), L"Test Commit");
    EXPECT_EQ(log5->getMessage(), L"");
    EXPECT_EQ(log5->getFullMessage(), str);
}

TEST_F(GitServiceTest, ParseGitBranch)
{
    std::wstring str = L"* master hashID Title 1";
    auto checkoutBranch1 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch1);
    EXPECT_EQ(checkoutBranch1->getName(), L"master");
    EXPECT_EQ(checkoutBranch1->getIsActive(), true);
    EXPECT_EQ(checkoutBranch1->getHashID(), L"hashID");
    EXPECT_EQ(checkoutBranch1->getTitle(), L"Title 1");

    str = L" master hashID Title 1";
    auto checkoutBranch2 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch2);
    EXPECT_EQ(checkoutBranch2->getName(), L"master");
    EXPECT_EQ(checkoutBranch2->getIsActive(), false);
    EXPECT_EQ(checkoutBranch2->getHashID(), L"hashID");
    EXPECT_EQ(checkoutBranch2->getTitle(), L"Title 1");

    str = L" master -> origin/Head";
    auto checkoutBranch3 = std::make_shared<GitBranch>();
    GitService::ParseGitBranch(str, checkoutBranch3);
    EXPECT_EQ(checkoutBranch3->getName(), L"master");
    EXPECT_EQ(checkoutBranch3->getIsActive(), false);
    EXPECT_EQ(checkoutBranch3->getHashID(), L"");
    EXPECT_EQ(checkoutBranch3->getTitle(), L"");
    EXPECT_EQ(checkoutBranch3->getPointToBranch(), L"origin/Head");
}

TEST_F(GitServiceTest, Tag)
{
    // init
    GitService::initializeGitResponse(this->getLogConfig().get(), this->getWorkspace());

    writeFile(concatPaths({this->getWorkspace(), L"test.txt"}), L"hi\r\n", true);
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Commit");
    // meaningless to test exception case
    // try
    // {
    //     GitTagCurrentTag noTag = GitService::getCurrentTag(this->getLogConfig().get(), this->getWorkspace());
    //     // show throw exception
    //     EXPECT_TRUE(false);
    // }
    // catch(...)
    // {
    //     EXPECT_TRUE(true);
    // }
    
    GitService::CreateTag(this->getLogConfig().get(), this->getWorkspace(), L"v0.0.1", nullptr);
    std::vector<std::wstring> tags = GitService::getTags(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(tags.size(), (size_t)1);
    EXPECT_EQ(tags[0], L"v0.0.1");
    auto currentTag = GitService::getCurrentTag(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(currentTag->getTagName(), L"v0.0.1");
    EXPECT_EQ(currentTag->getNoOfCommit(), 0);

    // Window behavior and Linux Behavior different, Window throw exception (tag will detach branch) while Linux will not
    // Can use GitService::SwitchReverse to switch back
    // As window will throw exception, meaningless to have unit test for switching tag
    // try {
    //     GitService::Switch(this->getLogConfig().get(), this->getWorkspace(), L"v0.0.1");
    // } catch (...) {

    // }
    //GitService::SwitchReverse(this->getLogConfig().get(), this->getWorkspace());

    // DECLARE_SPTR(GitLog, log);
    // GitService::getTag(this->getLogConfig().get(), this->getWorkspace(), L"v0.0.1", log);
    // EXPECT_EQ(log->getTags().size(), (size_t)1);
    // EXPECT_EQ(log->getTags().at(0), L"v0.0.1");

    GitService::DeleteTag(this->getLogConfig().get(), this->getWorkspace(), L"v0.0.1");
}

TEST_F(GitServiceTest, Branch)
{
    // init
    GitService::initializeGitResponse(this->getLogConfig().get(), this->getWorkspace());

    writeFile(concatPaths({this->getWorkspace(), L"test.txt"}), L"hi\r\n", true);
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Commit");
   
    // Create Branch
    GitService::CreateBranch(this->getLogConfig().get(), this->getWorkspace(), L"branch", nullptr);
    // main for linux, master for window
    EXPECT_TRUE(GitService::getCurrentBranchName(this->getLogConfig().get(), this->getWorkspace()) == L"main"
        || GitService::getCurrentBranchName(this->getLogConfig().get(), this->getWorkspace()) == L"master");

    // Current Branch
    // DECLARE_SPTR(GitBranch, currentbranch);
    // GitService::getCurrentBranch(this->getLogConfig().get(), this->getWorkspace(), currentbranch);
    // EXPECT_EQ(currentbranch->getName(), L"branch");
    // EXPECT_EQ(currentbranch->getIsActive(), false);
    // EXPECT_TRUE(!currentbranch->getHashID().empty());
    // EXPECT_TRUE(!currentbranch->getTitle().empty());
    // EXPECT_EQ(currentbranch->getPointToBranch(), L""); 

    // Switch Branch
    GitBranchSwitchBranchOption switchBranchOption;
    switchBranchOption.setIsQuite(true);
    GitService::SwitchBranch(this->getLogConfig().get(), this->getWorkspace(), L"branch", &switchBranchOption);
    // DECLARE_SPTR(GitBranch, switchBranch);
    // GitService::getCurrentBranch(this->getLogConfig().get(), this->getWorkspace(), switchBranch);
    // EXPECT_EQ(switchBranch->getName(), L"branch");
    // EXPECT_EQ(switchBranch->getIsActive(), true);
    // EXPECT_TRUE(!switchBranch->getHashID().empty());
    // EXPECT_TRUE(!switchBranch->getTitle().empty());
    // EXPECT_EQ(switchBranch->getPointToBranch(), L""); 
    EXPECT_EQ(GitService::getCurrentBranchName(this->getLogConfig().get(), this->getWorkspace()), L"branch");

    // get Branches
    auto branches = GitService::getBranches(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(branches.at(0)->getName(), L"branch");
    EXPECT_TRUE(branches.at(1)->getName() == L"main" || branches.at(1)->getName() == L"master");

    // Delete
    // no testing switch as window use master but linux use main
    //GitService::SwitchBranch(this->getLogConfig().get(), this->getWorkspace(), L"main");
    // cannot delete as still in main branch
    //GitService::DeleteBranch(this->getLogConfig().get(), this->getWorkspace(), L"branch");
}

TEST_F(GitServiceTest, parseGitDiff)
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
    auto diff = GitService::parseGitDiff(str);
    EXPECT_EQ(diff->getFilePathOld(), L"test.txt");
    EXPECT_EQ(diff->getFilePathNew(), L"test.txt");
    EXPECT_EQ(diff->getLineNumberOld()[0], (size_t)1);
    EXPECT_EQ(diff->getLineCountOld()[0], (size_t)0);
    EXPECT_EQ(diff->getLineNumberNew()[0], (size_t)1);
    EXPECT_EQ(diff->getLineCountNew()[0], (size_t)2);
    EXPECT_EQ(diff->getChangedLines()[0], expectedChangedLine);
}

TEST_F(GitServiceTest, stageAndDifference)
{
    // init
    GitService::initializeGitResponse(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_TRUE(std::filesystem::exists(this->getWorkspace() + L"/.git/HEAD"));
    
    // check existance
    EXPECT_TRUE(GitService::IsGitResponse(this->getLogConfig().get(), this->getWorkspace()));

    // config
    auto config = std::make_shared<GitConfig>();
    GitService::getLocalConfig(this->getLogConfig().get(), this->getWorkspace(), config);
    EXPECT_TRUE(config->getUserName().empty());
    EXPECT_TRUE(config->getUserEmail().empty());
    
    GitService::setLocalUserName(this->getLogConfig().get(), this->getWorkspace(), L"test");
    GitService::setLocalUserEmail(this->getLogConfig().get(), this->getWorkspace(), L"test@test.com");
    EXPECT_EQ(GitService::getLocalUserName(this->getLogConfig().get(), this->getWorkspace()), L"test");
    EXPECT_EQ(GitService::getLocalUserEmail(this->getLogConfig().get(), this->getWorkspace()), L"test@test.com");
    EXPECT_EQ(GitService::getUserName(this->getLogConfig().get(), this->getWorkspace()), L"test");
    EXPECT_EQ(GitService::getUserEmail(this->getLogConfig().get(), this->getWorkspace()), L"test@test.com");
    EXPECT_TRUE(GitService::IsLocalConfigExists(this->getLogConfig().get(), this->getWorkspace(), L"user.name"));

    // Case: Empty File
    auto statusEmpty = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    // main for linux, master for window
    EXPECT_TRUE(statusEmpty->getBranch() == L"main" || statusEmpty->getBranch() == L"master");
    EXPECT_EQ(statusEmpty->getRemoteBranch(), L"");

    // Case: New File
    writeFile(concatPaths({this->getWorkspace(), L"test.txt"}), L"hi\r\n", true);
    auto statusCreateFile = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusCreateFile->getIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusCreateFile->getWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->getWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->getWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test.txt");

    // Case: staged
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    auto statusNewState = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusNewState->getIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusNewState->getIndexFiles()[GitFileStatus::Added].size(), (size_t)1);
    EXPECT_EQ(statusNewState->getIndexFiles()[GitFileStatus::Added].at(0), L"test.txt");
    EXPECT_EQ(statusNewState->getWorkingTreeFiles().size(), (size_t)0);
    GitService::unstage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    GitService::stageAll(this->getLogConfig().get(), this->getWorkspace());
    GitService::unstageAll(this->getLogConfig().get(), this->getWorkspace());
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
 
    // Case: Committed
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Commit");
    auto statusNewCommit = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusNewCommit->getIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusNewCommit->getWorkingTreeFiles().size(), (size_t)0);
    
    // Case: Modified
    appendFileOneLine(concatPaths({this->getWorkspace(), L"test.txt"}), L"HI", false);
    auto statusModify = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusModify->getIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusModify->getWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModify->getWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModify->getWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    auto differentSummary = GitService::getDifferenceSummary(this->getLogConfig().get(), this->getWorkspace(), {});
    EXPECT_EQ(differentSummary->getFiles().size(), (size_t)1);
    EXPECT_EQ(differentSummary->getFiles().at(0), L"test.txt");
    EXPECT_EQ(differentSummary->getAddLineCounts().at(0), (size_t)1);
    EXPECT_EQ(differentSummary->getDeleteLineCounts().at(0), (size_t)0);
    auto diff = GitService::getDifferenceWorkingFile(this->getLogConfig().get(), this->getWorkspace(), L"test.txt"); // output in parseGitDiff
    EXPECT_EQ(diff->getFilePathOld(), L"test.txt");
    EXPECT_EQ(diff->getFilePathNew(), L"test.txt");
    EXPECT_EQ(diff->getLineNumberOld()[0], (size_t)1);
    EXPECT_EQ(diff->getLineCountOld()[0], (size_t)0);
    EXPECT_EQ(diff->getLineNumberNew()[0], (size_t)1);
    EXPECT_EQ(diff->getLineCountNew()[0], (size_t)2);
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    auto statusModifyState = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusModifyState->getIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState->getIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState->getIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState->getWorkingTreeFiles().size(), (size_t)0);
    appendFileOneLine(concatPaths({this->getWorkspace(), L"test.txt"}), L"BI", false);
    auto statusModifyState2 = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusModifyState2->getIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->getIndexFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->getIndexFiles()[GitFileStatus::Modified].at(0), L"test.txt");
    EXPECT_EQ(statusModifyState2->getWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->getWorkingTreeFiles()[GitFileStatus::Modified].size(), (size_t)1);
    EXPECT_EQ(statusModifyState2->getWorkingTreeFiles()[GitFileStatus::Modified].at(0), L"test.txt");

    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test.txt");
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Modify");

    // Case: Renamed
    std::filesystem::rename(concatPaths({this->getWorkspace(), L"test.txt"}), concatPaths({this->getWorkspace(), L"test2.txt"}));
    auto statusRename = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusRename->getIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusRename->getWorkingTreeFiles().size(), (size_t)2);
    EXPECT_EQ(statusRename->getWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusRename->getWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test2.txt");
    EXPECT_EQ(statusRename->getWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusRename->getWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test.txt");
    GitService::stageAll(this->getLogConfig().get(), this->getWorkspace());
    auto statusRenameState = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusRenameState->getIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusRenameState->getIndexFiles()[GitFileStatus::Renamed].size(), (size_t)1);
    EXPECT_EQ(statusRenameState->getIndexFiles()[GitFileStatus::Renamed].at(0), L"test.txt -> test2.txt");
    EXPECT_EQ(statusRenameState->getWorkingTreeFiles().size(), (size_t)0);
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Rename");

    // Case: Deleted
    removeFile(concatPaths({this->getWorkspace(), L"test2.txt"}));
    auto statusDelete = GitService::getStatus(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(statusDelete->getIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusDelete->getWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusDelete->getWorkingTreeFiles()[GitFileStatus::Deleted].size(), (size_t)1);
    EXPECT_EQ(statusDelete->getWorkingTreeFiles()[GitFileStatus::Deleted].at(0), L"test2.txt");
    GitService::stage(this->getLogConfig().get(), this->getWorkspace(), L"test2.txt");
    GitService::Commit(this->getLogConfig().get(), this->getWorkspace(), L"Test Delete");

    // Summary Log
    std::vector<std::shared_ptr<GitLog>> logs = GitService::getLogs(this->getLogConfig().get(), this->getWorkspace());
    EXPECT_EQ(logs.size(), (size_t)4);
    EXPECT_EQ(logs.at(0)->getTitle(), L"Test Delete");
    EXPECT_EQ(logs.at(1)->getTitle(), L"Test Rename");
    EXPECT_EQ(logs.at(2)->getTitle(), L"Test Modify");
    EXPECT_EQ(logs.at(3)->getTitle(), L"Test Commit");

    // Test normal operation
    GitService::FetchAll(this->getLogConfig().get(), this->getWorkspace());
    // Note: cannot pull as it is local response
    //GitService::Pull(this->getLogConfig().get(), this->getWorkspace(), nullptr);
}

// TEST_F(GitServiceTest, Config)
// {
//     DECLARE_SPTR(GitConfig, config);
//     GitService::getGlobalConfig(this->getLogConfig().get(), config);
//     std::wstring userName = GitService::getGlobalUserName(this->getLogConfig().get());
//     std::wstring userEmail = GitService::getGlobalUserEmail(this->getLogConfig().get());
//     EXPECT_TRUE(!config->getUserName().empty());
//     EXPECT_TRUE(!config->getUserEmail().empty());
//     EXPECT_EQ(config->getUserName(), userName);
//     EXPECT_EQ(config->getUserEmail(), userEmail);
// }
