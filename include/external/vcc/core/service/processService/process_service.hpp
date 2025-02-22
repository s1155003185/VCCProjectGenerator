#pragma once
#include "base_service.hpp"

#include <string>
#include <vector>

#include "log_config.hpp"

namespace vcc
{
    class ProcessService : public BaseService
    {
        private:
            #ifdef _WIN32
            static std::wstring _ExecuteWindow(const std::wstring &command);
            #else
            static std::wstring _ExecuteLinux(const std::string &command);
            #endif

            static std::wstring _Execute(const std::wstring &command);

        public:
            ProcessService() : BaseService() {}
            ~ProcessService() {}

            static std::vector<std::string> ParseCMDLinux(const std::string &cmd);
            static std::wstring Execute(const LogConfig *logConfig, const std::wstring &id, const std::wstring &command);
            static std::wstring Execute(const LogConfig *logConfig, const std::wstring &id, const std::wstring &workspace, const std::wstring &command);
    };
}