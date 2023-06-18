#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

#include "class_macro.hpp"
#include "git_service.hpp"
#include "log_property.hpp"

using namespace std;
using namespace vcc;

class GitServiceTest : public testing::Test 
{
    THREAD_SAFE
    GETOBJ(LogProperty, LogPropery);
    GET(wstring, Workspace, L"bin/Debug/Git/");
    public:

        void SetUp() override
        {
            this->_LogPropery.SetIsConsoleLog(false);
        }

        void TearDown() override
        {

        }
};

TEST_F(GitServiceTest, Version)
{
    std::wstring version = GitService::GetVersion(*this->GetLogPropery(), L"");
    EXPECT_TRUE(regex_match(version, wregex(L"[0-9]+.[0-9]+.[0-9]+")));
}

TEST_F(GitServiceTest, InitializeLocalResponse)
{
    // init
    GitService::InitializeWorkspace(*this->GetLogPropery(), L"", this->GetWorkspace());
    EXPECT_TRUE(filesystem::exists(this->GetWorkspace() + L"/.git/HEAD"));
    
    // check existance
    EXPECT_TRUE(GitService::IsGitResponse(*this->GetLogPropery(), this->GetWorkspace()));
}
