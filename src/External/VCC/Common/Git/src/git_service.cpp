#include "git_service.hpp"

#include <filesystem>
#include <regex>

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
    
    std::wstring GitService::GetVersion(LogProperty &logProperty)
    {
        std::wstring cmd = L"git --version";
        std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, cmd);
        
        std::wsmatch m;
        if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
            return m[0];
        return L"";
    }

    std::wstring GitService::InitializeWorkspace(LogProperty &logProperty, std::wstring workspace)
    {
        try {
            std::wstring cmd = L"git init";
            return ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
        } catch (Exception &ex) {
            THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
        }
        return L"";
    }

    std::wstring GitService::CloneResponse(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist)
    {
        try {
            std::wstring cmd = L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"");
            return ProcessService::Execute(logProperty, GIT_LOG_ID, dist, cmd);
        } catch (Exception &ex) {
            THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
            return L"";
        }
    }

    bool GitService::IsGitResponse(LogProperty &logProperty, std::wstring workspace)
    {
        bool result = false;
        try {
            std::wstring cmd = L"git rev-parse --git-dir";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, cmd);
            result = true;
        } catch (Exception &ex) {
            THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
        }
        return result;
    }
}