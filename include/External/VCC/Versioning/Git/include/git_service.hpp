#pragma once
#include "base_service.hpp"

#include "log_property.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    class GitService : public BaseService
    {
        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(LogProperty &logProperty, std::wstring command);

        // General
        static std::wstring GetVersion(LogProperty &logProperty);
        //static void GetConfig();


        // Validation
        static bool IsGitResponse(LogProperty &logProperty, std::wstring workspace);

        // Initialization
        static std::wstring Initialize(LogProperty &logProperty, std::wstring workspace);
        static std::wstring Clone(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist, int64_t depth = -1);
        static std::wstring CheckOut(LogProperty &logProperty, std::wstring workspace, std::wstring branch);

        // Action
        static std::wstring Pull(LogProperty &logProperty, std::wstring workspace);
    };
}
