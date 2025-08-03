#include "terminal_service.hpp"

#include <filesystem>

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring TerminalService::execute(const LogConfig *logConfig, std::wstring id, std::wstring cmd)
    {
        TRY
            return execute(logConfig, id, L"", cmd);
        CATCH
        return L"";
    }

    std::wstring TerminalService::execute(const LogConfig *logConfig, const std::wstring &id, const std::wstring &workspace, const std::wstring &cmd)
    {
        LogService::LogTerminal(logConfig, id, cmd);

        std::wstring currentDirectory = L"";
        std::wstring result = L"";
        try {
            currentDirectory = std::filesystem::current_path().wstring();
            if (!isBlank(workspace))
                std::filesystem::current_path(workspace);
        } catch (std::exception &e) {
            if (!isBlank(workspace))
                std::filesystem::current_path(currentDirectory);
            THROW_EXCEPTION(e);
            return result;
        }
        
        try {
            // main process
            char buffer[1024];
            FILE* p = popen(wstr2str(cmd).c_str(), "r");
            if (p == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot execute Command: " + cmd);
            try {            
                while (!feof(p)) {
                    if (fgets(buffer, sizeof(buffer), p) != nullptr)
                        result += str2wstr(buffer);
                }
                int status = pclose(p);
                #ifdef _WIN32
                if (status != 0)
                #else
                if (WEXITSTATUS(status) != 0)
                #endif
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, result);
            }
            catch(const std::exception& e) {
                pclose(p);
            }
        } catch (std::exception &e) {
            if (!isBlank(workspace))
                std::filesystem::current_path(currentDirectory);
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, str2wstr(e.what()));
        }

        try {
            if (!isBlank(workspace))
                std::filesystem::current_path(currentDirectory);
        } catch (std::exception &e) {
            THROW_EXCEPTION(e);
            return result;
        }

        LogService::LogTerminalResult(logConfig, id, result);

        trim(result);
        return result;
    }
};