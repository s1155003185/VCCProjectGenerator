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

TEST_F(GitServiceTest, FullTest)
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
    WriteFile(ConcatPath(this->GetWorkspace(), L"test.txt"), L"hi", true);
    DECLARE_SPTR(GitStatus, statusCreateFile);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusCreateFile);
    EXPECT_EQ(statusCreateFile->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles().size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].size(), (size_t)1);
    EXPECT_EQ(statusCreateFile->GetWorkingTreeFiles()[GitFileStatus::Untracked].at(0), L"test.txt");
 
    // Case: Staged
    GitService::Stage(this->GetLogProperty().get(), this->GetWorkspace(), L"test.txt");
    DECLARE_SPTR(GitStatus, statusState);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusState);
    EXPECT_EQ(statusState->GetIndexFiles().size(), (size_t)1);
    EXPECT_EQ(statusState->GetIndexFiles()[GitFileStatus::Added].size(), (size_t)1);
    EXPECT_EQ(statusState->GetIndexFiles()[GitFileStatus::Added].at(0), L"test.txt");
    EXPECT_EQ(statusState->GetWorkingTreeFiles().size(), (size_t)0);
 

    // Case: Committed
    GitService::Commit(this->GetLogProperty().get(), this->GetWorkspace(), L"Test Commit");
    DECLARE_SPTR(GitStatus, statusCommit);
    GitService::GetStatus(this->GetLogProperty().get(), this->GetWorkspace(), statusCommit);
    EXPECT_EQ(statusCommit->GetIndexFiles().size(), (size_t)0);
    EXPECT_EQ(statusCommit->GetWorkingTreeFiles().size(), (size_t)0);
    
    // Case: Modified

    // Case: Renamed

    // Case: Deleted
}
