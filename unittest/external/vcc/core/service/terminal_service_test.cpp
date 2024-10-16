#include <gtest/gtest.h>

#include "terminal_service.hpp"
#include "log_config.hpp"
#include "string_helper.hpp"

using namespace vcc;

TEST(TerminalServiceTest, Normal)
{
    EXPECT_TRUE(TerminalService::Execute(nullptr, L"", L"git --version").starts_with(L"git version"));
}

// Terminal cannot cap exception console log
// TEST(CommandTest, Exception)
// {
//     LogConfig logConfig(LogConfigType::None);
//     bool isException = false;
//     try {
//         TerminalService::Execute(logConfig, L"", "git -version");
//     } catch (...) {
//         isException = true;
//     }
//     EXPECT_TRUE(isException);
// }
