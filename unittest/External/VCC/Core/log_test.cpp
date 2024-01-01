#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "file_helper.hpp"
#include "log_service.hpp"

using namespace std;
using namespace vcc;

TEST(LogServiceTest, LogTest) 
{
    PATH filePath = std::filesystem::current_path() / "bin/Debug/AppLogs/LogServiceTest.log";
    remove(filePath);
    
    LogProperty property;
    property.SetUserID(L"user");
    property.SetIsConsoleLog(false);
    property.SetIsLogTerminal(true);
    property.SetIsLogTerminalResult(true);
    property.SetIsLogProcess(true);
    property.SetIsLogProcessResult(true);
    property.SetIsLogSQL(true);
    property.SetIsLogSQLResult(true);
    property.SetFilePath(filePath.wstring());
    std::wstring logInfoStr = LogService::LogInfo(property, L"id", L"message");
    std::wstring logWarningStr = LogService::LogWarning(property, L"id", L"message");
    std::wstring logErrorStr = LogService::LogError(property, L"id", L"message");
    std::wstring LogTerminalStr = LogService::LogTerminal(property, L"id", L"message");
    std::wstring LogTerminalResultStr = LogService::LogTerminalResult(property, L"id", L"message");
    std::wstring logProcessStr = LogService::LogProcess(property, L"id", L"message");
    std::wstring logProcessResultStr = LogService::LogProcessResult(property, L"id", L"message");
    std::wstring logSQLStr = LogService::LogSQL(property, L"id", L"message");
    std::wstring logSQLResultStr = LogService::LogSQLResult(property, L"id", L"message");
    EXPECT_EQ(logInfoStr, ReadFileOneLine(filePath.wstring(), 0));
    EXPECT_EQ(logWarningStr, ReadFileOneLine(filePath.wstring(), 1));
    EXPECT_EQ(logErrorStr, ReadFileOneLine(filePath.wstring(), 2));
    EXPECT_EQ(LogTerminalStr, ReadFileOneLine(filePath.wstring(), 3));
    EXPECT_EQ(LogTerminalResultStr, ReadFileOneLine(filePath.wstring(), 4));
    EXPECT_EQ(logProcessStr, ReadFileOneLine(filePath.wstring(), 5));
    EXPECT_EQ(logProcessResultStr, ReadFileOneLine(filePath.wstring(), 6));
    EXPECT_EQ(logSQLStr, ReadFileOneLine(filePath.wstring(), 7));
    EXPECT_EQ(logSQLResultStr, ReadFileOneLine(filePath.wstring(), 8));
}
