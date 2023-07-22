#include <gtest/gtest.h>

#include "process_service.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(ProcessTest, Normal)
{
    LogProperty logProperty(LogPropertyType::None);
    EXPECT_TRUE(HasPrefix(ProcessService::Execute(logProperty, L"", L"git --version"), L"git version"));
}

TEST(ProcessTest, NormalWithWorkspace)
{
    LogProperty logProperty(LogPropertyType::None);
    EXPECT_TRUE(HasPrefix(ProcessService::Execute(logProperty, L"", L"..", L"git --version"), L"git version"));
}

TEST(ProcessTest, Exception)
{
    LogProperty logProperty(LogPropertyType::None);

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
