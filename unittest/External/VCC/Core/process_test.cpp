#include <gtest/gtest.h>

#include "process_service.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(ProcessTest, Normal)
{
    EXPECT_TRUE(ProcessService::Execute(nullptr, L"", L"git --version").starts_with(L"git version"));
}

TEST(ProcessTest, NormalWithWorkspace)
{
    EXPECT_TRUE(ProcessService::Execute(nullptr, L"", L"..", L"git --version").starts_with(L"git version"));
}

TEST(ProcessTest, Exception)
{
    bool isError = false;
    try {
        ProcessService::Execute(nullptr, L"", L"git -version");
    } catch (exception &e) {
        string exceptStr(e.what());
        EXPECT_TRUE(exceptStr.find("unknown option") != string::npos);
        isError = true;
    }
    EXPECT_TRUE(isError);
}
