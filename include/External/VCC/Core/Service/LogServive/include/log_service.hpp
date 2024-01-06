#pragma once

#include <string>

#include "base_service.hpp"
#include "log_property.hpp"

namespace vcc
{
    enum class LogType
    {
        Error,
        Warning,
        Info
    };

	class LogService : public BaseService
	{
	private:
		static std::wstring _logMessage(const LogProperty &logProperty, LogType logType, std::wstring id, std::wstring message);

	public:
		LogService() : BaseService() {}
		~LogService() {}

		// General
		static std::wstring LogInfo(const LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogWarning(const LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogError(const LogProperty &logProperty, std::wstring id, std::wstring message);
		// Command
		static std::wstring LogTerminal(const LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogTerminalResult(const LogProperty &logProperty, std::wstring id, std::wstring message);
		// Process
		static std::wstring LogProcess(const LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogProcessResult(const LogProperty &logProperty, std::wstring id, std::wstring message);
		// SQL
		static std::wstring LogSQL(const LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogSQLResult(const LogProperty &logProperty, std::wstring id, std::wstring message);
	};
}
