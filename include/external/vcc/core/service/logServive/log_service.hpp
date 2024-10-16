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
		static std::wstring LogInfo(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogDebug(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogWarning(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogError(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Thread
		static std::wstring LogThread(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Terminal
		static std::wstring LogTerminal(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogTerminalResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// Process
		static std::wstring LogProcess(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogProcessResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		// SQL
		static std::wstring LogSQL(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
		static std::wstring LogSQLResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message);
	};
}
