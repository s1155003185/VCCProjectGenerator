#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "file_helper.hpp"
#include "log_service.hpp"

TEST(LogServiceTest, LogTest) 
{
    std::wstring filePath = vcc::concatPaths({std::filesystem::current_path().wstring(), L"bin/Debug/AppLogs/LogServiceTest.log"});
    vcc::removeFile(filePath);
    
    auto property = std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None);
    property->setUserID(L"user");
    property->setIsConsoleLog(false);
    property->setIsLogTerminal(true);
    property->setIsLogTerminalResult(true);
    property->setIsLogProcess(true);
    property->setIsLogProcessResult(true);
    property->setIsLogSQL(true);
    property->setIsLogSQLResult(true);
    property->setFilePath(filePath);
    std::wstring logInfoStr = vcc::LogService::logInfo(property.get(), L"id", L"message");
    std::wstring logWarningStr = vcc::LogService::LogWarning(property.get(), L"id", L"message");
    std::wstring logErrorStr = vcc::LogService::LogError(property.get(), L"id", L"message");
    std::wstring LogTerminalStr = vcc::LogService::LogTerminal(property.get(), L"id", L"message");
    std::wstring LogTerminalResultStr = vcc::LogService::LogTerminalResult(property.get(), L"id", L"message");
    std::wstring logProcessStr = vcc::LogService::LogProcess(property.get(), L"id", L"message");
    std::wstring logProcessResultStr = vcc::LogService::LogProcessResult(property.get(), L"id", L"message");
    std::wstring logSQLStr = vcc::LogService::LogSQL(property.get(), L"id", L"message");
    std::wstring logSQLResultStr = vcc::LogService::LogSQLResult(property.get(), L"id", L"message");
    EXPECT_EQ(logInfoStr, vcc::readFileOneLine(filePath, 0));
    EXPECT_EQ(logWarningStr, vcc::readFileOneLine(filePath, 1));
    EXPECT_EQ(logErrorStr, vcc::readFileOneLine(filePath, 2));
    EXPECT_EQ(LogTerminalStr, vcc::readFileOneLine(filePath, 3));
    EXPECT_EQ(LogTerminalResultStr, vcc::readFileOneLine(filePath, 4));
    EXPECT_EQ(logProcessStr, vcc::readFileOneLine(filePath, 5));
    EXPECT_EQ(logProcessResultStr, vcc::readFileOneLine(filePath, 6));
    EXPECT_EQ(logSQLStr, vcc::readFileOneLine(filePath, 7));
    EXPECT_EQ(logSQLResultStr, vcc::readFileOneLine(filePath, 8));
}
