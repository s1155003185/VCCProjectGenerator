#pragma once
#include "base_service.hpp"

#include "log_property.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    // CheckSum

    class GitConfig {

    };

    class GitService : public BaseService
    {
        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(LogProperty &logProperty, const std::wstring &command);

        // General
        static std::wstring GetVersion(LogProperty &logProperty);
        static bool IsGitResponse(LogProperty &logProperty, const std::wstring &workspace);

        // Config
        static GitConfig GetGlobalConfig(LogProperty &logProperty);
        static GitConfig GetLocalConfig(LogProperty &logProperty, const std::wstring &workspace);

        // Initialization
        static std::wstring Initialize(LogProperty &logProperty, const std::wstring &workspace);
        static std::wstring Clone(LogProperty &logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth = -1);
        static std::wstring CheckOut(LogProperty &logProperty, const std::wstring &workspace, const std::wstring &branch);

        // Action
        static std::wstring Pull(LogProperty &logProperty, const std::wstring &workspace);
    };
}
