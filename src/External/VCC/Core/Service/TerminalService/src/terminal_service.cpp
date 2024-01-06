#include "terminal_service.hpp"

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "log_property.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring TerminalService::Execute(const LogProperty &logProperty, std::wstring id, std::wstring cmd)
    {
        LogService::LogTerminal(logProperty, id, cmd);

        char buffer[1024];
        FILE* p = popen(wstr2str(cmd).c_str(), "r");
        if (p == nullptr)
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Cannot Execute Command: " + cmd);
    
        std::wstring result;
        try {
            while (!feof(p)) {
                if (fgets(buffer, sizeof(buffer), p) != nullptr)
                    result += str2wstr(buffer);
            }
        } catch (exception &e) {
            pclose(p);
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, str2wstr(e.what()));
        }
        int status = pclose(p);
        #ifdef _WIN32
        if (status != 0)
        #else
        if (WEXITSTATUS(status) != 0)
        #endif
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, result);
        LogService::LogTerminalResult(logProperty, id, result);

        Trim(result);
        return result;
    }
}