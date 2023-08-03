#pragma once

#include "base_property.hpp"
#include "class_macro.hpp"

#include "log_type.hpp"

#include <string>

namespace vcc
{
    enum class LogPropertyType
    {
        None,
        All
    };

    class LogProperty : public BaseProperty
    {
        // General
        GETSET(std::wstring, UserID, L"");
        
        GETSET(bool, IsConsoleLog, true);
        GETSET(std::wstring, FilePath, L"");
        // Command
        GETSET(bool, IsLogCommand, true);
        GETSET(bool, IsLogCommandResult, true);
        // Process
        GETSET(bool, IsLogProcess, true);
        GETSET(bool, IsLogProcessResult, true);
        // SQL
        GETSET(bool, IsLogSQL, true);
        GETSET(bool, IsLogSQLResult, true);
    public:
        LogProperty() {}
        LogProperty(LogPropertyType logPropertyType) {
            switch (logPropertyType)
            {
            case LogPropertyType::None:
                this->SetIsConsoleLog(false);
                this->SetFilePath(L"");

                this->SetIsLogCommand(false);
                this->SetIsLogCommandResult(false);

                this->SetIsLogProcess(false);
                this->SetIsLogProcessResult(false);

                this->SetIsLogSQL(false);
                this->SetIsLogSQLResult(false);
                break;
            case LogPropertyType::All:
                this->SetIsConsoleLog(true);

                this->SetIsLogCommand(true);
                this->SetIsLogCommandResult(true);

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
        ~LogProperty() {}
    };
}
