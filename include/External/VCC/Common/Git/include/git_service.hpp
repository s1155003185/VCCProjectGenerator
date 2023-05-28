#pragma once
#include "base_service.hpp"

#include "log_property.hpp"

namespace vcc
{
    class GitService : public BaseService
    {
        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(std::wstring command);
        static std::wstring Execute(LogProperty &logProperty, std::wstring userId, std::wstring command);

        static std::wstring GetVersion();
        static std::wstring GetVersion(LogProperty &logProperty, std::wstring userId);

        static void InitializeWorkspace(std::wstring workspace);
        static void InitializeWorkspace(LogProperty &logProperty, std::wstring userId, std::wstring workspace);
    
        static void CloneResponse(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist);
    };
}
