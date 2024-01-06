#pragma once
#include "base_service.hpp"

#include "class_macro.hpp"
#include "log_property.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    // CheckSum

    class GitConfig {
        std::wstring UserName;

    };

    class GitService : public BaseService
    {
        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(const LogProperty &logProperty, const std::wstring &command);

        // General
        static std::wstring GetVersion(const LogProperty &logProperty);
        static bool IsGitResponse(const LogProperty &logProperty, const std::wstring &workspace);

        // Config
        static GitConfig GetGlobalConfig(const LogProperty &logProperty);
        static GitConfig GetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace);

        // Initialization
        static std::wstring Initialize(const LogProperty &logProperty, const std::wstring &workspace);
        static std::wstring Clone(const LogProperty &logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth = -1);
        static std::wstring CheckOut(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &branch);

        // Action
        static std::wstring Pull(const LogProperty &logProperty, const std::wstring &workspace);
    };
}
