#pragma once
#include "base_service.hpp"

#include <string>

#include "log_config.hpp"

namespace vcc
{
    class TerminalService : public BaseService
    {
        public:
            TerminalService() : BaseService() {}
            ~TerminalService() {}

            static std::wstring Execute(const LogConfig *logProperty, std::wstring id, std::wstring cmd);
            static std::wstring Execute(const LogConfig *logProperty, const std::wstring &id, const std::wstring &workspace, const std::wstring &cmd);
    };
}