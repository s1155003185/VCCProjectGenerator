#include "git_service.hpp"

#include <filesystem>
#include <regex>
#include <string>

#include "exception_macro.hpp"
#include "log_property.hpp"
#include "process_service.hpp"
#include "string_helper.hpp"

namespace vcc
{   
    std::wstring GitService::Execute(LogProperty &logProperty, std::wstring command)
    {
        return ProcessService::Execute(logProperty, GIT_LOG_ID, command);
    }
    
    // General
    std::wstring GitService::GetVersion(LogProperty &logProperty)
    {
        try {
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    // Validation
    bool GitService::IsGitResponse(LogProperty &logProperty, std::wstring workspace)
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

    // Initialization
    std::wstring GitService::Initialize(LogProperty &logProperty, std::wstring workspace)
    {
        try {
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git init" + (!workspace.empty() ? (L" " + workspace) : L""));
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    std::wstring GitService::Clone(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist, int64_t depth)
    {
        try {
            return ProcessService::Execute(logProperty, GIT_LOG_ID, dist, 
                L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"") + (depth > 0 ? (L" --depth " + std::to_wstring(depth)) : L""));
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    std::wstring GitService::CheckOut(LogProperty &logProperty, std::wstring workspace, std::wstring branch)
    {
        try {
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + branch);
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    // Action
    std::wstring GitService::Pull(LogProperty &logProperty, std::wstring workspace)
    {
        try {
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull");
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }
}