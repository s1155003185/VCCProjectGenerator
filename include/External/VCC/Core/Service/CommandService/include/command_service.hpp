#pragma once
#include "base_service.hpp"

#include <string>

#include "log_property.hpp"

namespace vcc
{
    class CommandService : public BaseService
    {
        public:
            CommandService() : BaseService() {}
            ~CommandService() {}

            static std::wstring Execute(std::wstring cmd);
            static std::wstring Execute(LogProperty &logProperty, std::wstring id, std::wstring userId, std::wstring cmd);
    };
}