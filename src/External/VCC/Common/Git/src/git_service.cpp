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


    void GitService::CloneResponse(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring dist)
    {
        std::wstring cmd = L"";
        trim(dist);
        std::wstring currentDirectory = std::filesystem::current_path().wstring();

        try {
            if (!dist.empty())
                std::filesystem::current_path(dist);

            std::wstring cmd = L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"");
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"", cmd);

        } catch (Exception &ex) {
            THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
        } catch (exception &ex) {
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, str2wstr(std::string(ex.what())));
        }
        if (!dist.empty())
            std::filesystem::current_path(currentDirectory);
    }

    bool GitService::IsGitResponse(LogProperty &logProperty, std::wstring workspace)
    {
        std::wstring cmd = L"";
        trim(workspace);
        std::wstring currentDirectory = std::filesystem::current_path().wstring();

        bool result = false;
        try {
            if (!workspace.empty())
                std::filesystem::current_path(workspace);

            std::wstring cmd = L"git rev-parse --git-dir";
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"", cmd);
            result = true;
        } catch (Exception &ex) {
            THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
        } catch (exception &ex) {
            THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, str2wstr(std::string(ex.what())));
        }
        if (!workspace.empty())
            std::filesystem::current_path(currentDirectory);
        return result;
    }
}