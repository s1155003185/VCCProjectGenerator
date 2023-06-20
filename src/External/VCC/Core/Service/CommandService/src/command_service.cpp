#include "command_service.hpp"

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "log_property.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring CommandService::Execute(LogProperty &logProperty, std::wstring id, std::wstring cmd)
    {
        LogService::LogCommand(logProperty, id, cmd);

        char buffer[1024];
        FILE* p = popen(wstr2str(cmd).c_str(), "r");
        if (p == nullptr)
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, L"Cannot Execute Command: " + cmd);
    
        std::wstring result;
        try {
            while (!feof(p)) {
                if (fgets(buffer, sizeof(buffer), p) != nullptr)
                    result += str2wstr(buffer);
            }
        } catch (exception &e) {
            pclose(p);
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, str2wstr(e.what()));
        }
        int status = pclose(p);
        #ifdef _WIN32
        if (status != 0)
        #else
        if (WEXITSTATUS(status) != 0)
        #endif
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, result);
        LogService::LogCommandResult(logProperty, id, result);
        return result;
    }
}