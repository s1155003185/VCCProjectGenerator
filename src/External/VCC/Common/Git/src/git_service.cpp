#include "git_service.hpp"

#include <regex>

#include "git_constant.hpp"
#include "log_property.hpp"
#include "process_service.hpp"
#include "string_helper.hpp"

namespace vcc
{
    std::wstring GitService::Execute(std::wstring command)
    {
        LogProperty defaultLogProperty;
        return GitService::Execute(defaultLogProperty, L"", command);
    }
    
    std::wstring GitService::Execute(LogProperty &logProperty, std::wstring userId, std::wstring command)
    {
        return ProcessService::Execute(logProperty, GIT_LOG_ID, userId, command);
    }

    std::wstring GitService::GetVersion()
    {
        LogProperty defaultLogProperty;
        return GitService::GetVersion(defaultLogProperty, L"");
    }
    
    std::wstring GitService::GetVersion(LogProperty &logProperty, std::wstring userId)
    {
        std::wstring cmd = L"git --version";
        std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, userId, cmd);
        
        std::wsmatch m;
        if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
            return m[0];
        return L"";
    }

    void GitService::InitializeWorkspace(std::wstring workspace)
    {
        LogProperty defaultLogProperty;
        GitService::InitializeWorkspace(defaultLogProperty, L"", workspace);
    }

    void GitService::InitializeWorkspace(LogProperty &logProperty, std::wstring userId, std::wstring workspace)
    {
        std::wstring cmd = L"git init";
        if (!workspace.empty())
            cmd += L" " + workspace;
        ProcessService::Execute(logProperty, GIT_LOG_ID, userId, cmd);
    }
}