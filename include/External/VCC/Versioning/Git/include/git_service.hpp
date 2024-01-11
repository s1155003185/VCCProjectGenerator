#pragma once
#include "base_service.hpp"

#include <memory>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    // CheckSum

    class GitConfig : public BaseObject {
        GETSET(std::wstring, UserName, L"");
        GETSET(std::wstring, UserEmail, L"");
        MAP(std::wstring, std::wstring, Configs);

        public:
            GitConfig() : BaseObject() {}
            virtual ~GitConfig() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitConfig>();
            }
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

        // Global Config
        static bool IsConfigExists(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetConfig(const LogProperty &logProperty, const std::wstring &workspace, GitConfig &config);
        static std::wstring GetConfig(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key);
        static std::wstring GetUserName(const LogProperty &logProperty, const std::wstring &workspace);
        static std::wstring GetUserEmail(const LogProperty &logProperty, const std::wstring &workspace);
        
        static bool IsGlobalConfigExists(const LogProperty &logProperty, const std::wstring &key);
        static void GetGlobalConfig(const LogProperty &logProperty, GitConfig &config);
        static std::wstring GetGlobalConfig(const LogProperty &logProperty, const std::wstring &key);
        static void SetGlobalConfig(const LogProperty &logProperty, const std::wstring &key, const std::wstring &value);
        
        static std::wstring GetGlobalUserName(const LogProperty &logProperty);
        static void SetGlobalUserName(const LogProperty &logProperty, const std::wstring &value);
        static std::wstring GetGlobalUserEmail(const LogProperty &logProperty);
        static void SetGlobalUserEmail(const LogProperty &logProperty, const std::wstring &value);

        // Local Config
        static bool IsLocalConfigExists(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace, GitConfig &config);
        static std::wstring GetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key);
        static void SetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value);

        static std::wstring GetLocalUserName(const LogProperty &logProperty, const std::wstring &workspace);
        static void SetLocalUserName(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &value);
        static std::wstring GetLocalUserEmail(const LogProperty &logProperty, const std::wstring &workspace);
        static void SetLocalUserEmail(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &value);

        // Initialization
        static std::wstring Initialize(const LogProperty &logProperty, const std::wstring &workspace);
        static std::wstring Clone(const LogProperty &logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth = -1);
        static std::wstring CheckOut(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &branch);

        // Action
        static std::wstring Pull(const LogProperty &logProperty, const std::wstring &workspace);
    };
}
