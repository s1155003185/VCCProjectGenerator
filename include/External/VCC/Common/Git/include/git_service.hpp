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

        static std::wstring Execute(LogProperty &logProperty, std::wstring command);

        static std::wstring GetVersion(LogProperty &logProperty);

        static std::wstring InitializeWorkspace(LogProperty &logProperty, std::wstring workspace);
    
        static std::wstring CloneResponse(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist);

        static bool IsGitResponse(LogProperty &logProperty, std::wstring workspace);
    };
}
