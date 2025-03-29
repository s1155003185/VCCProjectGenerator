#include "log_service.hpp"

#include <iostream>

#include "time_helper.hpp"
#include "file_helper.hpp"

namespace vcc
{
	std::wstring LogService::_logMessage(const LogConfig *logConfig, const LogType &logType, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr)
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
		case LogType::Debug:
			logMessage = L"[DBUG]";
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
		if (!IsBlank(logConfig->GetUserID()))
			logMessage += L" [" + logConfig->GetUserID() + L"] ";
		logMessage += L" " + message;
		if (logConfig->GetIsConsoleLog())
			std::wcout << logMessage << std::endl;

		if (!IsBlank(logConfig->GetFilePath())) {
			AppendFileOneLine(logConfig->GetFilePath(), logMessage, true);
		}
		return logMessage;
	}

	std::wstring LogService::LogInfo(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogDebug(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (!logConfig->GetIsLogDebug())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Debug, id, message);
	}

	std::wstring LogService::LogWarning(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		return LogService::_logMessage(logConfig, LogType::Warning, id, message);
	}

	std::wstring LogService::LogError(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		return LogService::_logMessage(logConfig, LogType::Error, id, message);
	}

	std::wstring LogService::LogThread(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogThread())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogTerminal(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogTerminal())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogTerminalResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogTerminalResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcess(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogProcess())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcessResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogProcessResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQL(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogSQL())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQLResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->GetIsLogSQLResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}
}
