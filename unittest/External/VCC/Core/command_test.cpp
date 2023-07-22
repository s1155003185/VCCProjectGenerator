#include <gtest/gtest.h>

#include "command_service.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(CommandTest, Normal)
{
    LogProperty logProperty(LogPropertyType::None);
    EXPECT_TRUE(HasPrefix(CommandService::Execute(logProperty, L"", L"git --version"), L"git version"));
}

// Terminal cannot cap exception console log
// TEST(CommandTest, Exception)
// {
//     LogProperty logProperty(LogPropertyType::None);
//     bool isException = false;
//     try {
//         CommandService::Execute(logProperty, L"", "git -version");
//     } catch (...) {
//         isException = true;
//     }
//     EXPECT_TRUE(isException);
// }
