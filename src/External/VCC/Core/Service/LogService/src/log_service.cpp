#include "log_service.hpp"

#include <iostream>

#include "datetime_helper.hpp"
#include "file_helper.hpp"

namespace vcc
{
	std::wstring LogService::_logMessage(LogProperty &logProperty, LogType logType, std::wstring id, std::wstring userid, std::wstring message)
	{
		std::wstring logMessage;
		switch (logType)
		{
		case LogType::Error:
			logMessage = L"[ERRO]";
			break;
		case LogType::Warning:
			logMessage = L"[WARN]";
			break;
		case LogType::Info:
			logMessage = L"[INFO]";
			break;
		default:
			logMessage = L"[INFO]";
			break;
		}

		logMessage += L" " + GetCurrentDatetimeString();
		if (!id.empty())
			logMessage += L" [" + id + L"]";
		if (!userid.empty())
			logMessage += L" [" + userid + L"] ";
		logMessage += L" " + message;
		if (logProperty.GetIsConsoleLog())
			std::wcout << logMessage << endl;

		if (!logProperty.GetFilePath().empty()) {
			AppendFileSingleLline(logProperty.GetFilePath(), logMessage, true);
		}
		return logMessage;
	}

	std::wstring LogService::LogInfo(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogInfo(defaultProperty, message);
	}

	std::wstring LogService::LogInfo(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogInfo(defaultProperty, id, message);
	}

	std::wstring LogService::LogInfo(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogInfo(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogInfo(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogInfo(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogInfo(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogInfo(logProperty, id, L"", message);
	}

	std::wstring LogService::LogInfo(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogWarning(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogWarning(defaultProperty, message);
	}

	std::wstring LogService::LogWarning(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogWarning(defaultProperty, id, message);
	}

	std::wstring LogService::LogWarning(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogWarning(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogWarning(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogWarning(logProperty, L"", message);
	}

	std::wstring LogService::LogWarning(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogWarning(logProperty, id, L"", message);
	}

	std::wstring LogService::LogWarning(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Warning, id, userid, message);
	}

	std::wstring LogService::LogError(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogError(defaultProperty, message);
	}

	std::wstring LogService::LogError(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogError(defaultProperty, id, message);
	}

	std::wstring LogService::LogError(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogError(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogError(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogError(logProperty, L"", message);
	}

	std::wstring LogService::LogError(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogError(logProperty, id, L"", message);
	}

	std::wstring LogService::LogError(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Error, id, userid, message);
	}

	std::wstring LogService::LogCommand(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommand(defaultProperty, message);
	}

	std::wstring LogService::LogCommand(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommand(defaultProperty, id, message);
	}

	std::wstring LogService::LogCommand(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommand(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogCommand(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogCommand(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogCommand(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogCommand(logProperty, id, L"", message);
	}

	std::wstring LogService::LogCommand(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogCommand())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogCommandResult(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommandResult(defaultProperty, message);
	}

	std::wstring LogService::LogCommandResult(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommandResult(defaultProperty, id, message);
	}

	std::wstring LogService::LogCommandResult(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogCommandResult(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogCommandResult(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogCommandResult(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogCommandResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogCommandResult(logProperty, id, L"", message);
	}

	std::wstring LogService::LogCommandResult(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogCommandResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogProcess(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcess(defaultProperty, message);
	}

	std::wstring LogService::LogProcess(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcess(defaultProperty, id, message);
	}

	std::wstring LogService::LogProcess(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcess(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogProcess(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogProcess(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogProcess(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogProcess(logProperty, id, L"", message);
	}

	std::wstring LogService::LogProcess(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogProcess())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogProcessResult(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcessResult(defaultProperty, message);
	}

	std::wstring LogService::LogProcessResult(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcessResult(defaultProperty, id, message);
	}

	std::wstring LogService::LogProcessResult(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogProcessResult(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogProcessResult(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogProcessResult(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogProcessResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogProcessResult(logProperty, id, L"", message);
	}

	std::wstring LogService::LogProcessResult(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogProcessResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogSQL(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQL(defaultProperty, message);
	}

	std::wstring LogService::LogSQL(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQL(defaultProperty, id, message);
	}

	std::wstring LogService::LogSQL(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQL(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogSQL(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogSQL(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogSQL(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogSQL(logProperty, id, L"", message);
	}

	std::wstring LogService::LogSQL(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogSQL())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}

	std::wstring LogService::LogSQLResult(std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQLResult(defaultProperty, message);
	}

	std::wstring LogService::LogSQLResult(std::wstring id, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQLResult(defaultProperty, id, message);
	}

	std::wstring LogService::LogSQLResult(std::wstring id, std::wstring userid, std::wstring message)
	{
		LogProperty defaultProperty;
		return LogService::LogSQLResult(defaultProperty, id, userid, message);
	}

	std::wstring LogService::LogSQLResult(LogProperty &logProperty, std::wstring message)
	{
		return LogService::LogSQLResult(logProperty, L"", L"", message);
	}

	std::wstring LogService::LogSQLResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::LogSQLResult(logProperty, id, L"", message);
	}

	std::wstring LogService::LogSQLResult(LogProperty &logProperty, std::wstring id, std::wstring userid, std::wstring message)
	{
		if (!logProperty.GetIsLogSQLResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, userid, message);
	}
}
