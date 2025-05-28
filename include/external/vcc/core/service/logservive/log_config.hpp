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
            Initialize(logConfigType, filePath);
        }
        virtual ~LogConfig() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<LogConfig>(*this);
        }

        void Initialize(const LogConfigInitialType &logConfigType, const std::wstring &filePath = L"") {
            switch (logConfigType)
            {
            case LogConfigInitialType::None:
                this->SetIsConsoleLog(false);
                this->SetFilePath(L"");

                this->SetIsLogDebug(false);

                this->SetIsLogThread(false);

                this->SetIsLogTerminal(false);
                this->SetIsLogTerminalResult(false);

                this->SetIsLogProcess(false);
                this->SetIsLogProcessResult(false);

                this->SetIsLogSQL(false);
                this->SetIsLogSQLResult(false);
                break;
            case LogConfigInitialType::All:
                this->SetIsConsoleLog(true);
                this->SetFilePath(filePath);
                assert(!filePath.empty());

                this->SetIsLogDebug(true);
                
                this->SetIsLogThread(true);

                this->SetIsLogTerminal(true);
                this->SetIsLogTerminalResult(true);

                this->SetIsLogProcess(true);
                this->SetIsLogProcessResult(true);

                this->SetIsLogSQL(true);
                this->SetIsLogSQLResult(true);
                break;            
            default:
                assert(false);
                break;
            }
        }
    };
}
