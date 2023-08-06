#include "log_service.hpp"

#include <iostream>

#include "datetime_helper.hpp"
#include "file_helper.hpp"

namespace vcc
{
	std::wstring LogService::_logMessage(LogProperty &logProperty, LogType logType, std::wstring id, std::wstring message)
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
		if (!logProperty.GetUserID().empty())
			logMessage += L" [" + logProperty.GetUserID() + L"] ";
		logMessage += L" " + message;
		if (logProperty.GetIsConsoleLog())
			std::wcout << logMessage << std::endl;

		if (!logProperty.GetFilePath().empty()) {
			AppendFileSingleLine(logProperty.GetFilePath(), logMessage, true);
		}
		return logMessage;
	}

	std::wstring LogService::LogInfo(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogWarning(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Warning, id, message);
	}

	std::wstring LogService::LogError(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Error, id, message);
	}

	std::wstring LogService::LogCommand(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogCommand())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogCommandResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogCommandResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcess(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogProcess())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcessResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogProcessResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQL(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogSQL())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQLResult(LogProperty &logProperty, std::wstring id, std::wstring message)
	{
		if (!logProperty.GetIsLogSQLResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}
}
