#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "process_service.hpp"
#include "log_config.hpp"
#include "string_helper.hpp"

TEST(ProcessServiceTest, Normal)
{
    EXPECT_TRUE(vcc::ProcessService::execute(nullptr, L"", L"git --version").starts_with(L"git version"));
}

TEST(ProcessServiceTest, NormalWithWorkspace)
{
    EXPECT_TRUE(vcc::ProcessService::execute(nullptr, L"", L"..", L"git --version").starts_with(L"git version"));
}

TEST(ProcessServiceTest, ParseCMDToken)
{
    std::vector<std::string> tokens = vcc::ProcessService::ParseCmdLinux("");
    std::vector<std::string> expectedTokens;
    EXPECT_EQ(tokens, expectedTokens);

    tokens = vcc::ProcessService::ParseCmdLinux("a");
    expectedTokens.clear();
    expectedTokens.push_back("a");
    EXPECT_EQ(tokens, expectedTokens);

    tokens = vcc::ProcessService::ParseCmdLinux("a b");
    expectedTokens.clear();
    expectedTokens.push_back("a");
    expectedTokens.push_back("b");
    EXPECT_EQ(tokens, expectedTokens);

    tokens = vcc::ProcessService::ParseCmdLinux("a \"a b c\"");
    expectedTokens.clear();
    expectedTokens.push_back("a");
    expectedTokens.push_back("a b c");
    EXPECT_EQ(tokens, expectedTokens);

    tokens = vcc::ProcessService::ParseCmdLinux("a \"a \\\"b\\\" c\"");
    expectedTokens.clear();
    expectedTokens.push_back("a");
    expectedTokens.push_back("a \"b\" c");
    EXPECT_EQ(tokens, expectedTokens);

    tokens = vcc::ProcessService::ParseCmdLinux("a 'a \\\'b\\\' c'");
    expectedTokens.clear();
    expectedTokens.push_back("a");
    expectedTokens.push_back("a 'b' c");
    EXPECT_EQ(tokens, expectedTokens);
}

TEST(ProcessServiceTest, Exception)
{
    bool isError = false;
    try {
        vcc::ProcessService::execute(nullptr, L"", L"git -version");
    } catch (std::exception &e) {
        std::string exceptStr(e.what());
        EXPECT_TRUE(exceptStr.find("unknown option") != std::string::npos);
        isError = true;
    }
    EXPECT_TRUE(isError);
}
