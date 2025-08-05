#pragma once

#include <assert.h>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"

namespace vcc
{
    enum class LogConfigInitialType
    {
        None,
        All
    };

    class LogConfig : public BaseObject
    {
        // General
        GETSET(std::wstring, UserID, L"");
        
        GETSET(bool, IsConsoleLog, false);
        GETSET(std::wstring, FilePath, L"");
        // Debug
        GETSET(bool, IsLogDebug, false);
        // Thread
        GETSET(bool, IsLogThread, false);
        // Terminal
        GETSET(bool, IsLogTerminal, false);
        GETSET(bool, IsLogTerminalResult, false);
        // Process
        GETSET(bool, IsLogProcess, false);
        GETSET(bool, IsLogProcessResult, false);
        // SQL
        GETSET(bool, IsLogSQL, false);
        GETSET(bool, IsLogSQLResult, false);
    public:
        LogConfig() {}
        LogConfig(const LogConfigInitialType &logConfigType, const std::wstring &filePath = L"") {
            initialize(logConfigType, filePath);
        }
        virtual ~LogConfig() {}

        virtual std::shared_ptr<IObject> clone() const override
        {
            return std::make_shared<LogConfig>(*this);
        }

        void initialize(const LogConfigInitialType &logConfigType, const std::wstring &filePath = L"") {
            switch (logConfigType)
            {
            case LogConfigInitialType::None:
                this->setIsConsoleLog(false);
                this->setFilePath(L"");

                this->setIsLogDebug(false);

                this->setIsLogThread(false);

                this->setIsLogTerminal(false);
                this->setIsLogTerminalResult(false);

                this->setIsLogProcess(false);
                this->setIsLogProcessResult(false);

                this->setIsLogSQL(false);
                this->setIsLogSQLResult(false);
                break;
            case LogConfigInitialType::All:
                this->setIsConsoleLog(true);
                this->setFilePath(filePath);
                assert(!filePath.empty());

                this->setIsLogDebug(true);
                
                this->setIsLogThread(true);

                this->setIsLogTerminal(true);
                this->setIsLogTerminalResult(true);

                this->setIsLogProcess(true);
                this->setIsLogProcessResult(true);

                this->setIsLogSQL(true);
                this->setIsLogSQLResult(true);
                break;            
            default:
                assert(false);
                break;
            }
        }
    };
}
