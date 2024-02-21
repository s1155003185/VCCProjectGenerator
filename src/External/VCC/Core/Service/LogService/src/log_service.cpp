#include "log_service.hpp"

#include <iostream>

#include "time_helper.hpp"
#include "file_helper.hpp"

namespace vcc
{
	std::wstring LogService::_logMessage(const LogProperty *logProperty, LogType logType, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr)
			return L"";

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
		if (!IsBlank(id))
			logMessage += L" [" + id + L"]";
		if (!IsBlank(logProperty->GetUserID()))
			logMessage += L" [" + logProperty->GetUserID() + L"] ";
		logMessage += L" " + message;
		if (logProperty->GetIsConsoleLog())
			std::wcout << logMessage << std::endl;

		if (!IsBlank(logProperty->GetFilePath())) {
			AppendFileOneLine(logProperty->GetFilePath(), logMessage, true);
		}
		return logMessage;
	}

	std::wstring LogService::LogInfo(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogWarning(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Warning, id, message);
	}

	std::wstring LogService::LogError(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		return LogService::_logMessage(logProperty, LogType::Error, id, message);
	}

	std::wstring LogService::LogTerminal(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogTerminal())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogTerminalResult(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogTerminalResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcess(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogProcess())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcessResult(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogProcessResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQL(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogSQL())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQLResult(const LogProperty *logProperty, std::wstring id, std::wstring message)
	{
		if (logProperty == nullptr || !logProperty->GetIsLogSQLResult())
			return L"";
		return LogService::_logMessage(logProperty, LogType::Info, id, message);
	}
}
