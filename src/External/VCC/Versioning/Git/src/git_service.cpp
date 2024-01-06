#include "git_service.hpp"

#include <filesystem>
#include <regex>
#include <string>

#include <iostream>

#include "exception_macro.hpp"
#include "log_property.hpp"
#include "process_service.hpp"
#include "string_helper.hpp"

namespace vcc
{   
    std::wstring GitService::Execute(LogProperty &logProperty, const std::wstring &command)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, command);
        )
        return L"";
    }
    
    std::wstring GitService::GetVersion(LogProperty &logProperty)
    {
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        )
        return L"";
    }

    bool GitService::IsGitResponse(LogProperty &logProperty, const std::wstring &workspace)
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

    GitConfig GitService::GetGlobalConfig(LogProperty &logProperty)
    {
        GitConfig config;
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global --list");
            std::wcout << cmdResult << std::endl;
        )
        return config;
    }

    GitConfig GitService::GetLocalConfig(LogProperty &logProperty, const std::wstring &workspace)
    {
        GitConfig config;
        TRY_CATCH(
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --global --list");
            std::wcout << cmdResult << std::endl;
        )
        return config;
    }

    // Initialization
    std::wstring GitService::Initialize(LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git init");
        )
        return L"";
    }

    std::wstring GitService::Clone(LogProperty &logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, dist, 
                L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"") + (depth > 0 ? (L" --depth " + std::to_wstring(depth)) : L""));
        )
        return L"";
    }

    std::wstring GitService::CheckOut(LogProperty &logProperty, const std::wstring &workspace, const std::wstring &branch)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + branch);
        )
        return L"";
    }

    // Action
    std::wstring GitService::Pull(LogProperty &logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull");
        )
        return L"";
    }
}