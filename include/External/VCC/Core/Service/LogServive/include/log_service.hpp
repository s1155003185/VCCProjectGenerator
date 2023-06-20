#pragma once
#include "base_service.hpp"

#include <string>

#include "log_property.hpp"
#include "log_type.hpp"

namespace vcc
{
	class LogService : public BaseService
	{
	private:
		static std::wstring _logMessage(LogProperty &logProperty, LogType logType, std::wstring id, std::wstring message);

	public:
		LogService() : BaseService() {}
		~LogService() {}

		// General
		static std::wstring LogInfo(LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogWarning(LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogError(LogProperty &logProperty, std::wstring id, std::wstring message);
		// Command
		static std::wstring LogCommand(LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogCommandResult(LogProperty &logProperty, std::wstring id, std::wstring message);
		// Process
		static std::wstring LogProcess(LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogProcessResult(LogProperty &logProperty, std::wstring id, std::wstring message);
		// SQL
		static std::wstring LogSQL(LogProperty &logProperty, std::wstring id, std::wstring message);
		static std::wstring LogSQLResult(LogProperty &logProperty, std::wstring id, std::wstring message);
	};
}
