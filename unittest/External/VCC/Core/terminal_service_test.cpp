#include <gtest/gtest.h>

#include "terminal_service.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(TerminalTest, Normal)
{
    EXPECT_TRUE(TerminalService::Execute(nullptr, L"", L"git --version").starts_with(L"git version"));
}

// Terminal cannot cap exception console log
// TEST(CommandTest, Exception)
// {
//     LogProperty logProperty(LogPropertyType::None);
//     bool isException = false;
//     try {
//         TerminalService::Execute(logProperty, L"", "git -version");
//     } catch (...) {
//         isException = true;
//     }
//     EXPECT_TRUE(isException);
// }
