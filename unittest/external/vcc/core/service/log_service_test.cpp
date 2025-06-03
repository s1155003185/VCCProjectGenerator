#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "file_helper.hpp"
#include "log_service.hpp"

TEST(LogServiceTest, LogTest) 
{
    std::wstring filePath = vcc::ConcatPaths({std::filesystem::current_path().wstring(), L"bin/Debug/AppLogs/LogServiceTest.log"});
    vcc::RemoveFile(filePath);
    
    auto property = std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None);
    property->SetUserID(L"user");
    property->SetIsConsoleLog(false);
    property->SetIsLogTerminal(true);
    property->SetIsLogTerminalResult(true);
    property->SetIsLogProcess(true);
    property->SetIsLogProcessResult(true);
    property->SetIsLogSQL(true);
    property->SetIsLogSQLResult(true);
    property->SetFilePath(filePath);
    std::wstring logInfoStr = vcc::LogService::LogInfo(property.get(), L"id", L"message");
    std::wstring logWarningStr = vcc::LogService::LogWarning(property.get(), L"id", L"message");
    std::wstring logErrorStr = vcc::LogService::LogError(property.get(), L"id", L"message");
    std::wstring LogTerminalStr = vcc::LogService::LogTerminal(property.get(), L"id", L"message");
    std::wstring LogTerminalResultStr = vcc::LogService::LogTerminalResult(property.get(), L"id", L"message");
    std::wstring logProcessStr = vcc::LogService::LogProcess(property.get(), L"id", L"message");
    std::wstring logProcessResultStr = vcc::LogService::LogProcessResult(property.get(), L"id", L"message");
    std::wstring logSQLStr = vcc::LogService::LogSQL(property.get(), L"id", L"message");
    std::wstring logSQLResultStr = vcc::LogService::LogSQLResult(property.get(), L"id", L"message");
    EXPECT_EQ(logInfoStr, vcc::ReadFileOneLine(filePath, 0));
    EXPECT_EQ(logWarningStr, vcc::ReadFileOneLine(filePath, 1));
    EXPECT_EQ(logErrorStr, vcc::ReadFileOneLine(filePath, 2));
    EXPECT_EQ(LogTerminalStr, vcc::ReadFileOneLine(filePath, 3));
    EXPECT_EQ(LogTerminalResultStr, vcc::ReadFileOneLine(filePath, 4));
    EXPECT_EQ(logProcessStr, vcc::ReadFileOneLine(filePath, 5));
    EXPECT_EQ(logProcessResultStr, vcc::ReadFileOneLine(filePath, 6));
    EXPECT_EQ(logSQLStr, vcc::ReadFileOneLine(filePath, 7));
    EXPECT_EQ(logSQLResultStr, vcc::ReadFileOneLine(filePath, 8));
}
