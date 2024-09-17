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
		Debug,
        Info
    };

	class LogService : public BaseService
	{
	private:
		static std::wstring _logMessage(const LogProperty *logProperty, const LogType &logType, const std::wstring &id, const std::wstring &message);

	public:
		LogService() : BaseService() {}
		~LogService() {}

		// General
		static std::wstring LogInfo(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogDebug(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogWarning(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogError(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		// Thread
		static std::wstring LogThread(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		// Terminal
		static std::wstring LogTerminal(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogTerminalResult(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		// Process
		static std::wstring LogProcess(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogProcessResult(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		// SQL
		static std::wstring LogSQL(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
		static std::wstring LogSQLResult(const LogProperty *logProperty, const std::wstring &id, const std::wstring &message);
	};
}
