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
    property.SetIsLogCommand(true);
    property.SetIsLogCommandResult(true);
    property.SetIsLogProcess(true);
    property.SetIsLogProcessResult(true);
    property.SetIsLogSQL(true);
    property.SetIsLogSQLResult(true);
    property.SetFilePath(filePath);
    std::wstring logInfoStr = LogService::LogInfo(property, L"id", L"message");
    std::wstring logWarningStr = LogService::LogWarning(property, L"id", L"message");
    std::wstring logErrorStr = LogService::LogError(property, L"id", L"message");
    std::wstring logCommandStr = LogService::LogCommand(property, L"id", L"message");
    std::wstring logCommandResultStr = LogService::LogCommandResult(property, L"id", L"message");
    std::wstring logProcessStr = LogService::LogProcess(property, L"id", L"message");
    std::wstring logProcessResultStr = LogService::LogProcessResult(property, L"id", L"message");
    std::wstring logSQLStr = LogService::LogSQL(property, L"id", L"message");
    std::wstring logSQLResultStr = LogService::LogSQLResult(property, L"id", L"message");
    EXPECT_EQ(logInfoStr, ReadFileSingleLline(filePath, 0));
    EXPECT_EQ(logWarningStr, ReadFileSingleLline(filePath, 1));
    EXPECT_EQ(logErrorStr, ReadFileSingleLline(filePath, 2));
    EXPECT_EQ(logCommandStr, ReadFileSingleLline(filePath, 3));
    EXPECT_EQ(logCommandResultStr, ReadFileSingleLline(filePath, 4));
    EXPECT_EQ(logProcessStr, ReadFileSingleLline(filePath, 5));
    EXPECT_EQ(logProcessResultStr, ReadFileSingleLline(filePath, 6));
    EXPECT_EQ(logSQLStr, ReadFileSingleLline(filePath, 7));
    EXPECT_EQ(logSQLResultStr, ReadFileSingleLline(filePath, 8));
}
