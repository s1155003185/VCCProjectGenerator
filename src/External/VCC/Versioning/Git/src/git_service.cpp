#include "git_service.hpp"

#include <filesystem>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

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

    GitFileStatus GitService::ParseGitFileStatus(const wchar_t &c)
    {
        TRY_CATCH(
            switch (c)
            {
                case L' ':
                    return GitFileStatus::NA;
                case L'!':
                    return GitFileStatus::Ignored;
                case L'?':
                    return GitFileStatus::Untracked;
                case L'M':
                    return GitFileStatus::Updated;
                case L'T':
                    return GitFileStatus::TypeChanged;
                case L'A':
                    return GitFileStatus::Added;
                case L'D':
                    return GitFileStatus::Deleted;
                case L'R':
                    return GitFileStatus::Renamed;
                case L'C':
                    return GitFileStatus::Copied;                
                default:
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown Git file status " + std::wstring(1, c));
            }
        )
        return GitFileStatus::Ignored;
    }
    
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

    void GitService::GetStatus(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitStatus> status, bool isWithIgnoredFiles)
    {
        TRY_CATCH(
            std::wstring withIgnoredFilesOption = isWithIgnoredFiles ? L" --ignored" : L"";
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git status -b -s" + withIgnoredFilesOption));
            
            std::wstring localBrachPrefix = L"## No commits yet on ";
            std::wstring remoteBranchPrefix = L"## ";
            size_t prefixLength = 2;
            for (auto line : lines) {
                Trim(line);
                if (line.length() <= 2)
                    continue;

                // for branch
                if (HasPrefix(line, localBrachPrefix)) {
                    // Note: For non-remote git response, no commit return is L"## No commits yet on main"
                    // Note: For remote git response, no commit return is L"## main...origin/main"
                    std::wstring subLine = line.substr(localBrachPrefix.length());
                    Trim(subLine);
                    status->SetBranch(subLine);
                    status->SetRemoteBranch(L"");
                    continue;
                } else if (HasPrefix(line, remoteBranchPrefix)) {
                    std::vector<std::wstring> tokens = SplitString(line.substr(remoteBranchPrefix.length()), L"...");
                    status->SetBranch(tokens.at(0));
                    if (tokens.size() > 1)
                        status->SetRemoteBranch(tokens.at(1));
                    continue;
                }

                // for files
                GitFileStatus indexFileStatus = GitService::ParseGitFileStatus(line[0]);
                GitFileStatus workingTreeFileStatus = GitService::ParseGitFileStatus(line[1]);
                std::wstring filePath = line.size() > prefixLength ? line.substr(prefixLength) : L"";
                Trim(filePath);
                if (indexFileStatus == GitFileStatus::Ignored || indexFileStatus == GitFileStatus::Untracked) {
                    status->GetWorkingTreeFiles()[indexFileStatus].push_back(filePath);
                    continue;
                }
                if (indexFileStatus != GitFileStatus::NA)
                    status->GetIndexFiles()[indexFileStatus].push_back(filePath);
                if (workingTreeFileStatus != GitFileStatus::NA)
                    status->GetWorkingTreeFiles()[workingTreeFileStatus].push_back(filePath);
            }
        )
    }

    void GitService::Pull(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull");
        )
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
            for (auto tmpConfig : globalResult->GetConfigs()) {
                if (config->GetConfigs().find(tmpConfig.first) != config->GetConfigs().end())
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
                for (auto it : element->GetConfigs()) {
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
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
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
                for (auto it : element->GetConfigs()) {
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
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
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

    void GitService::Stage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
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