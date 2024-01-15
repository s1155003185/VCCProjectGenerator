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
                std::shared_ptr<GitConfig> obj = std::make_shared<GitConfig>(*this);
                obj->CloneConfigs(*this->GetConfigs().get());
                return obj;
            }
    };

    class GitStatus : public BaseObject {
        GETSET(std::wstring, Branch, L"");

        public:
            GitStatus() : BaseObject() {}
            virtual ~GitStatus() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitStatus>();
            }
    };

    class GitService : public BaseService
    {
        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(const LogProperty *logProperty, const std::wstring &command);

        // General
        static std::wstring GetVersion(const LogProperty *logProperty);
        static bool IsGitResponse(const LogProperty *logProperty, const std::wstring &workspace);

        // Global Config
        static bool IsConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static std::wstring GetUserName(const LogProperty *logProperty, const std::wstring &workspace);
        static std::wstring GetUserEmail(const LogProperty *logProperty, const std::wstring &workspace);
        
        static bool IsGlobalConfigExists(const LogProperty *logProperty, const std::wstring &key);
        static void GetGlobalConfig(const LogProperty *logProperty, std::shared_ptr<GitConfig> config);
        static std::wstring GetGlobalConfig(const LogProperty *logProperty, const std::wstring &key);
        static void SetGlobalConfig(const LogProperty *logProperty, const std::wstring &key, const std::wstring &value);
        
        static std::wstring GetGlobalUserName(const LogProperty *logProperty);
        static void SetGlobalUserName(const LogProperty *logProperty, const std::wstring &value);
        static std::wstring GetGlobalUserEmail(const LogProperty *logProperty);
        static void SetGlobalUserEmail(const LogProperty *logProperty, const std::wstring &value);

        // Local Config
        static bool IsLocalConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void SetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value);

        static std::wstring GetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace);
        static void SetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value);
        static std::wstring GetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace);
        static void SetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value);

        // Initialization
        static void Initialize(const LogProperty *logProperty, const std::wstring &workspace);
        static void Clone(const LogProperty *logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth = -1);
        static void CheckOut(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branch);

        // Refresh
        static void GetStatus(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitStatus> status);
        static void Pull(const LogProperty *logProperty, const std::wstring &workspace);
        
        // Commit
        static void AddToStage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath);
        static void Commit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &command);

    };
}
