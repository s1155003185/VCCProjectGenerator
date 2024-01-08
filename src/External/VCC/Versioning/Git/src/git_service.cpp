#include "git_service.hpp"

#include <filesystem>
#include <memory>
#include <regex>
#include <string>

#include "config_reader.hpp"
#include "exception_macro.hpp"
#include "log_property.hpp"
#include "process_service.hpp"
#include "string_helper.hpp"

namespace vcc
{   
    #define GIT_CONFIG_USER_NAME L"user.name"
    #define GIT_CONFIG_USER_EMAIL L"user.email"

    std::wstring GitService::Execute(const LogProperty &logProperty, const std::wstring &command)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, command);
        )
        return L"";
    }
    
    std::wstring GitService::GetVersion(const LogProperty &logProperty)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        )
        return L"";
    }

    bool GitService::IsGitResponse(const LogProperty &logProperty, const std::wstring &workspace)
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

    GitConfig GitService::GetGlobalConfig(const LogProperty &logProperty)
    {
        GitConfig config;
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global --list");
            if (!IsEmptyOrWhitespace(cmdResult)) {
                ConfigElement element;
                ConfigReader reader;
                reader.Parse(cmdResult, element);
                for (auto it : *element.GetConfigs()) {
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config.SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config.SetUserEmail(it.second);
                    }
                    config.InsertConfigs(it.first, it.second);
                }

            }
        )
        return config;
    }

    std::wstring GitService::GetGlobalConfig(const LogProperty &logProperty, const std::wstring &key)
    {
        TRY_CATCH(
            return SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key)).at(0);
        )
        return L"";
    }
    
    void GitService::SetGlobalConfig(const LogProperty &logProperty, const std::wstring &key, const std::wstring &value)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        )
    }

    std::wstring GitService::GetGlobalUserName(const LogProperty &logProperty)
    {
        TRY_CATCH(
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME);
        )
        return L"";
    }

    void GitService::SetGlobalUserName(const LogProperty &logProperty, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME, value);
        )
    }

    std::wstring GitService::GetGlobalUserEmail(const LogProperty &logProperty)
    {
        TRY_CATCH(
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL);
        )
        return L"";
    }

    void GitService::SetGlobalUserEmail(const LogProperty &logProperty, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL, value);
        )
    }

    GitConfig GitService::GetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace)
    {
        GitConfig config;
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local --list");
            if (!IsEmptyOrWhitespace(cmdResult)) {
                ConfigElement element;
                ConfigReader reader;
                reader.Parse(cmdResult, element);
                for (auto it : *element.GetConfigs()) {
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config.SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config.SetUserEmail(it.second);
                    }
                    config.InsertConfigs(it.first, it.second);
                }
            }
        )
        return config;
    }

    std::wstring GitService::GetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        TRY_CATCH(
            return SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key)).at(0);
        )
        return L"";
    }

    void GitService::SetLocalConfig(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        )
    }

    std::wstring GitService::GetLocalUserName(const LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME);
        )
        return L"";
    }

    void GitService::SetLocalUserName(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME, value);
        )
    }

    std::wstring GitService::GetLocalUserEmail(const LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL);
        )
        return L"";
    }

    void GitService::SetLocalUserEmail(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY_CATCH(
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL, value);
        )
    }

    // Initialization
    std::wstring GitService::Initialize(const LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git init");
        )
        return L"";
    }

    std::wstring GitService::Clone(const LogProperty &logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, dist, 
                L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"") + (depth > 0 ? (L" --depth " + std::to_wstring(depth)) : L""));
        )
        return L"";
    }

    std::wstring GitService::CheckOut(const LogProperty &logProperty, const std::wstring &workspace, const std::wstring &branch)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + branch);
        )
        return L"";
    }

    // Action
    std::wstring GitService::Pull(const LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull");
        )
        return L"";
    }
}