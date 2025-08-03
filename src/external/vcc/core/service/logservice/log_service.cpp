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

		logMessage += L" " + getCurrentDatetimeString();
		if (!isBlank(id))
			logMessage += L" [" + id + L"]";
		if (!isBlank(logConfig->getUserID()))
			logMessage += L" [" + logConfig->getUserID() + L"] ";
		logMessage += L" " + message;
		if (logConfig->getIsConsoleLog())
			std::wcout << logMessage << std::endl;

		if (!isBlank(logConfig->getFilePath())) {
			appendFileOneLine(logConfig->getFilePath(), logMessage, true);
		}
		return logMessage;
	}

	std::wstring LogService::logInfo(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogDebug(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (!logConfig->getIsLogDebug())
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
		if (logConfig == nullptr || !logConfig->getIsLogThread())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogTerminal(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogTerminal())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogTerminalResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogTerminalResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcess(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogProcess())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogProcessResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogProcessResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQL(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogSQL())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}

	std::wstring LogService::LogSQLResult(const LogConfig *logConfig, const std::wstring &id, const std::wstring &message)
	{
		if (logConfig == nullptr || !logConfig->getIsLogSQLResult())
			return L"";
		return LogService::_logMessage(logConfig, LogType::Info, id, message);
	}
}
