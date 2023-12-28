#include "git_service.hpp"

#include <filesystem>
#include <regex>
#include <string>

#include "exception_macro.hpp"
#include "git_constant.hpp"
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
        std::wstring cmd = L"git --version";
        std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, cmd);
        
        std::wsmatch m;
        if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
            return m[0];
        return L"";
    }

    // Validation
    bool GitService::IsGitResponse(LogProperty &logProperty, std::wstring workspace)
    {
        bool result = false;
        try {
            std::wstring cmd = L"git rev-parse --is-inside-work-tree";
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
            result = cmdResult.find(L"true") != std::wstring::npos;
        } catch (...) {
            result = false;
        }
        return result;
    }

    // Initialization
    std::wstring GitService::InitializeWorkspace(LogProperty &logProperty, std::wstring workspace)
    {
        try {
            std::wstring cmd = L"git init";
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    std::wstring GitService::CloneResponse(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist)
    {
        try {
            std::wstring cmd = L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"");
            return ProcessService::Execute(logProperty, GIT_LOG_ID, dist, cmd);
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    std::wstring GitService::CheckOut(LogProperty &logProperty, std::wstring workspace, std::wstring branch)
    {
        try {
            std::wstring cmd = L"git checkout " + branch;
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    // Action
    std::wstring GitService::Pull(LogProperty &logProperty, std::wstring workspace)
    {
        try {
            std::wstring cmd = L"git pull";
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }
}