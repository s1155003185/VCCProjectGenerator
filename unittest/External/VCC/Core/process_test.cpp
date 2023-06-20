#include <gtest/gtest.h>

#include "process_service.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(ProcessTest, Normal)
{
    LogProperty logProperty;
    logProperty.SetIsConsoleLog(false);
    logProperty.SetIsLogCommand(false);
    logProperty.SetIsLogCommandResult(false);
    EXPECT_TRUE(HasPrefix(ProcessService::Execute(logProperty, L"", L"git --version"), L"git version"));
}

TEST(ProcessTest, Exception)
{
    LogProperty logProperty;
    logProperty.SetIsConsoleLog(false);
    logProperty.SetIsLogCommand(false);
    logProperty.SetIsLogCommandResult(false);

    bool isError = false;
    try {
        ProcessService::Execute(logProperty, L"", L"git -version");
    } catch (exception &e) {
        string exceptStr(e.what());
        EXPECT_TRUE(exceptStr.find("unknown option") != string::npos);
        isError = true;
    }
    EXPECT_TRUE(isError);
}