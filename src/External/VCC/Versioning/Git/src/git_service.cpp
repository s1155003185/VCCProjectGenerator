#include "git_service.hpp"

#include <filesystem>
#include <memory>
#include <regex>
#include <string>

#include "config_reader.hpp"
#include "exception_macro.hpp"
#include "log_property.hpp"
#include "memory_macro.hpp"
#include "process_service.hpp"
#include "string_helper.hpp"

namespace vcc
{   
    #define GIT_CONFIG_USER_NAME L"user.name"
    #define GIT_CONFIG_USER_EMAIL L"user.email"

    std::wstring GitService::Execute(const LogProperty *logProperty, const std::wstring &command)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, command);
        )
        return L"";
    }
    
    std::wstring GitService::GetVersion(const LogProperty *logProperty)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        )
        return L"";
    }

    bool GitService::IsGitResponse(const LogProperty *logProperty, const std::wstring &workspace)
    {
        bool result = false;
        try {
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace,
                L"git rev-parse --is-inside-work-tree");
            result = cmdResult.find(L"true") != std::wstring::npos;
        } catch (...) {
            result = false;
         }
        return result;
    }

    bool GitService::IsConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        bool result = false;
        try {
            GitService::GetConfig(logProperty, workspace, key);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;
    }

    void GitService::GetConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config)
    {
        TRY_CATCH(
            GitService::GetLocalConfig(logProperty, workspace, config);
            DECLARE_SPTR(GitConfig, globalResult);
            GitService::GetGlobalConfig(logProperty, globalResult);
            if (config->GetUserName().empty())
                config->SetUserName(globalResult->GetUserName());
            if (config->GetUserEmail().empty())
                config->SetUserEmail(globalResult->GetUserEmail());
            for (auto tmpConfig : *globalResult->GetConfigs()) {
                if (config->GetConfigs()->find(tmpConfig.first) != config->GetConfigs()->end())
                    config->InsertConfigs(tmpConfig.first, tmpConfig.second);
            }
        )
    }

    std::wstring GitService::GetConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        std::wstring result = L"";
        TRY_CATCH(
            try {
                result = GitService::GetLocalConfig(logProperty, workspace, key);           
            } catch (...) {
                result = L"";
            }
            if (result.empty())
                result = GitService::GetGlobalConfig(logProperty, key);
        )
        return result;
    }

    std::wstring GitService::GetUserName(const LogProperty *logProperty, const std::wstring &workspace)
    {
        std::wstring result = L"";
        TRY_CATCH(
            result = GitService::GetConfig(logProperty, workspace, GIT_CONFIG_USER_NAME);
        )
        return result;        
    }
    
    std::wstring GitService::GetUserEmail(const LogProperty *logProperty, const std::wstring &workspace)
    {
        std::wstring result = L"";
        TRY_CATCH(
            result = GitService::GetConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL);
        )
        return result;
    }

    bool GitService::IsGlobalConfigExists(const LogProperty *logProperty, const std::wstring &key)
    {
        bool result = false;
        try {
            DECLARE_SPTR(GitConfig, config);
            GitService::GetConfig(logProperty, key, config);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;        
    }

    void GitService::GetGlobalConfig(const LogProperty *logProperty, std::shared_ptr<GitConfig> config)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global --list");
            if (!IsEmptyOrWhitespace(cmdResult)) {
                DECLARE_SPTR(ConfigElement, element);
                DECLARE_UPTR(ConfigReader, reader);
                reader->Parse(cmdResult, element);
                for (auto it : *element->GetConfigs()) {
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->SetUserEmail(it.second);
                    }
                    config->InsertConfigs(it.first, it.second);
                }

            }
        )
    }

    std::wstring GitService::GetGlobalConfig(const LogProperty *logProperty, const std::wstring &key)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key);
            return IsEmptyOrWhitespace(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        )
        return L"";
    }
    
    void GitService::SetGlobalConfig(const LogProperty *logProperty, const std::wstring &key, const std::wstring &value)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key + L" " + value);
        )
    }

    std::wstring GitService::GetGlobalUserName(const LogProperty *logProperty)
    {
        TRY_CATCH(
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME);
        )
        return L"";
    }

    void GitService::SetGlobalUserName(const LogProperty *logProperty, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME, value);
        )
    }

    std::wstring GitService::GetGlobalUserEmail(const LogProperty *logProperty)
    {
        TRY_CATCH(
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL);
        )
        return L"";
    }

    void GitService::SetGlobalUserEmail(const LogProperty *logProperty, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL, value);
        )
    }

    bool GitService::IsLocalConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        bool result = false;
        try {
            GitService::GetLocalConfig(logProperty, workspace, key);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;       
    }

    void GitService::GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local --list");
            if (!IsEmptyOrWhitespace(cmdResult)) {
                DECLARE_SPTR(ConfigElement, element);
                DECLARE_UPTR(ConfigReader, reader);

                reader->Parse(cmdResult, element);
                for (auto it : *element->GetConfigs()) {
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->SetUserEmail(it.second);
                    }
                    config->InsertConfigs(it.first, it.second);
                }
            }
        )
    }

    std::wstring GitService::GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key);
            return IsEmptyOrWhitespace(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        )
        return L"";
    }

    void GitService::SetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key + L" " + value);
        )
    }

    std::wstring GitService::GetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME);
        )
        return L"";
    }

    void GitService::SetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME, value);
        )
    }

    std::wstring GitService::GetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL);
        )
        return L"";
    }

    void GitService::SetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL, value);
        )
    }

    // Initialization
    void GitService::Initialize(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git init");
        )
    }

    void GitService::Clone(const LogProperty *logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, dist, 
                L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"") + (depth > 0 ? (L" --depth " + std::to_wstring(depth)) : L""));
        )
    }

    void GitService::CheckOut(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branch)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + branch);
        )
    }

    void GitService::Pull(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull");
        )
    }

    void GitService::AddToStage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git add " + filePath);
        )
    }

    void GitService::Commit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &command)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git commit -m \"" + GetEscapeString(EscapeStringType::DoubleQuote, command) + L"\"");
        )
    }
}