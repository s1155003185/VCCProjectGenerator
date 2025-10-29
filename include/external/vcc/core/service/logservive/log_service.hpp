#pragma once

#include <string>

#include "base_service.hpp"
#include "log_config.hpp"

namespace vcc
{
    enum class LogType
    {
        Error,
        Warning,
		Debug,
        Info
    };

	class LogService : public BaseService
	{
	private:
		static std::wstring _logMessage(const LogConfig *logConfig, const LogType &logType, const std::wstring &id, const std::wstring &message);

	public:
		LogService() : BaseService() {}
		~LogService() {}

		// General
		static std::wstring logInfo(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logDebug(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logWarning(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logError(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Thread
		static std::wstring logThread(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Terminal
		static std::wstring logTerminal(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logTerminalResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Process
		static std::wstring logProcess(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logProcessResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// SQL
		static std::wstring logSQL(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring logSQLResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
	};
}
