#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "file_helper.hpp"
#include "log_service.hpp"

using namespace vcc;

TEST(LogServiceTest, LogTest) 
{
    std::wstring filePath = ConcatPaths({std::filesystem::current_path().wstring(), L"bin/Debug/AppLogs/LogServiceTest.log"});
    RemoveFile(filePath);
    
    std::shared_ptr<LogConfig> property = std::make_shared<LogConfig>(LogConfigInitialType::None);
    property->SetUserID(L"user");
    property->SetIsConsoleLog(false);
    property->SetIsLogTerminal(true);
    property->SetIsLogTerminalResult(true);
    property->SetIsLogProcess(true);
    property->SetIsLogProcessResult(true);
    property->SetIsLogSQL(true);
    property->SetIsLogSQLResult(true);
    property->SetFilePath(filePath);
    std::wstring logInfoStr = LogService::LogInfo(property.get(), L"id", L"message");
    std::wstring logWarningStr = LogService::LogWarning(property.get(), L"id", L"message");
    std::wstring logErrorStr = LogService::LogError(property.get(), L"id", L"message");
    std::wstring LogTerminalStr = LogService::LogTerminal(property.get(), L"id", L"message");
    std::wstring LogTerminalResultStr = LogService::LogTerminalResult(property.get(), L"id", L"message");
    std::wstring logProcessStr = LogService::LogProcess(property.get(), L"id", L"message");
    std::wstring logProcessResultStr = LogService::LogProcessResult(property.get(), L"id", L"message");
    std::wstring logSQLStr = LogService::LogSQL(property.get(), L"id", L"message");
    std::wstring logSQLResultStr = LogService::LogSQLResult(property.get(), L"id", L"message");
    EXPECT_EQ(logInfoStr, ReadFileOneLine(filePath, 0));
    EXPECT_EQ(logWarningStr, ReadFileOneLine(filePath, 1));
    EXPECT_EQ(logErrorStr, ReadFileOneLine(filePath, 2));
    EXPECT_EQ(LogTerminalStr, ReadFileOneLine(filePath, 3));
    EXPECT_EQ(LogTerminalResultStr, ReadFileOneLine(filePath, 4));
    EXPECT_EQ(logProcessStr, ReadFileOneLine(filePath, 5));
    EXPECT_EQ(logProcessResultStr, ReadFileOneLine(filePath, 6));
    EXPECT_EQ(logSQLStr, ReadFileOneLine(filePath, 7));
    EXPECT_EQ(logSQLResultStr, ReadFileOneLine(filePath, 8));
}
