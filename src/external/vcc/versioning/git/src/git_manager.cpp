#include "git_manager.hpp"

#include <string>

#include "exception_macro.hpp"
#include "git_service.hpp"

namespace vcc
{
    GitManager::GitManager(std::shared_ptr<LogConfig> logConfig, const std::wstring &workspace) : BaseManager(logConfig)
    {
        _Workspace = workspace;
    }

    void GitManager::validate() const
    {
        TRY
            VALIDATE(L"Workspace is empty", !IsBlank(_Workspace))
        CATCH
    }

    std::wstring GitManager::GetVersion() const
    {
        TRY
            return GitService::GetVersion(_LogConfig.get());
        CATCH
        return L"";
    }

    bool GitManager::IsGitResponse() const
    {
        TRY
            validate();
            return GitService::IsGitResponse(_LogConfig.get(), _Workspace);
        CATCH
        return false;
    }

    std::shared_ptr<GitStatus> GitManager::GetStatus(const GitStatusSearchCriteria *searchCriteria) const
    {
        TRY
            validate();
            return GitService::GetStatus(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }

    void GitManager::InitializeGitResponse()
    {
        TRY
            validate();
            GitService::InitializeGitResponse(_LogConfig.get(), _Workspace);
        CATCH
    }

    void GitManager::cloneGitResponse(const std::wstring &url, const GitCloneOption *option)
    {
        TRY
            validate();
            GitService::cloneGitResponse(_LogConfig.get(), _Workspace, url, option);
        CATCH
    }

    std::vector<std::shared_ptr<GitRemote>> GitManager::GetRemote()
    {
        TRY
            validate();
            return GitService::GetRemote(_LogConfig.get(), _Workspace);
        CATCH
        return {};
    }

    void GitManager::AddRemote(const std::wstring &name, const std::wstring &url, const GitRemoteMirror &mirror)
    {
        TRY
            validate();
            GitService::AddRemote(_LogConfig.get(), _Workspace, name, url, mirror);
        CATCH
    }
    
    void GitManager::RenameRemote(const std::wstring &oldName, const std::wstring &newName)
    {
        TRY
            validate();
            GitService::RenameRemote(_LogConfig.get(), _Workspace, oldName, newName);
        CATCH
    }
    
    void GitManager::RemoveRemote(const std::wstring &name)
    {
        TRY
            validate();
            GitService::RemoveRemote(_LogConfig.get(), _Workspace, name);
        CATCH
    }
    
    void GitManager::FetchAll()
    {
        TRY
            validate();
            GitService::FetchAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Pull(const GitPullOption *option)
    {
        TRY
            validate();
            GitService::Pull(_LogConfig.get(), _Workspace, option);
        CATCH
    }
    
    void GitManager::Push(const GitPushOption *option)
    {
        TRY
            validate();
            GitService::Push(_LogConfig.get(), _Workspace, option);
        CATCH
    }
    
    std::vector<std::shared_ptr<GitLog>> GitManager::GetLogs(const GitLogSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetLogs(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::vector<std::wstring> GitManager::GetTags(const GitTagSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetTags(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::shared_ptr<GitTagCurrentTag> GitManager::GetCurrentTag()
    {
        TRY
            validate();
            return GitService::GetCurrentTag(_LogConfig.get(), _Workspace);
        CATCH
        return nullptr;
    }
    
    void GitManager::CreateTag(const std::wstring &tagName, const GitTagCreateTagOption *option)
    {
        TRY
            validate();
            GitService::CreateTag(_LogConfig.get(), _Workspace, tagName, option);
        CATCH
    }
    
    void GitManager::Switch(const std::wstring &tagName, bool isForce)
    {
        TRY
            validate();
            GitService::Switch(_LogConfig.get(), _Workspace, tagName, isForce);
        CATCH
    }
    
    void GitManager::DeleteTag(const std::wstring &tagName)
    {
        TRY
            validate();
            GitService::DeleteTag(_LogConfig.get(), _Workspace, tagName);
        CATCH
    }
    
    std::wstring GitManager::GetCurrentBranchName()
    {
        TRY
            validate();
            return GitService::GetCurrentBranchName(_LogConfig.get(), _Workspace);
        CATCH
        return L"";
    }
    
    std::vector<std::shared_ptr<GitBranch>> GitManager::GetBranches()
    {
        TRY
            validate();
            return GitService::GetBranches(_LogConfig.get(), _Workspace);
        CATCH
        return {};
    }
    
    void GitManager::CreateBranch( const std::wstring &branchName, const GitBranchCreateBranchOption *option)
    {
        TRY
            validate();
            GitService::CreateBranch(_LogConfig.get(), _Workspace, branchName, option);
        CATCH
    }
    
    void GitManager::SwitchBranch(const std::wstring &branchName, const GitBranchSwitchBranchOption *option)
    {
        TRY
            validate();
            GitService::SwitchBranch(_LogConfig.get(), _Workspace, branchName, option);
        CATCH
    }
    
    void GitManager::RenameBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            validate();
            GitService::RenameBranch(_LogConfig.get(), _Workspace, oldBranchName, newBranchName, isForce);
        CATCH
    }
    
    void GitManager::CopyBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            validate();
            GitService::CopyBranch(_LogConfig.get(), _Workspace, oldBranchName, newBranchName, isForce);
        CATCH
    }
    
    void GitManager::DeleteBranch(const std::wstring &branchName, bool isForce)
    {
        TRY
            validate();
            GitService::DeleteBranch(_LogConfig.get(), _Workspace, branchName, isForce);
        CATCH
    }
    
    std::shared_ptr<GitDifferenceSummary> GitManager::GetDifferenceSummary(const std::vector<std::wstring> &hashIDs)
    {
        TRY
            validate();
            return GitService::GetDifferenceSummary(_LogConfig.get(), _Workspace, hashIDs);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceIndexFile(const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetDifferenceIndexFile(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceWorkingFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetDifferenceWorkingFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetDifferenceFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitManager::GetDifferenceCommit(const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::GetDifferenceCommit(_LogConfig.get(), _Workspace, fromHashID, toHashID, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    void GitManager::Stage(const std::wstring &filePath)
    {
        TRY
            validate();
            GitService::Stage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::StageAll()
    {
        TRY
            validate();
            GitService::StageAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Unstage(const std::wstring &filePath)
    {
        TRY
            validate();
            GitService::Unstage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::UnstageAll()
    {
        TRY
            validate();
            GitService::UnstageAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Commit(const std::wstring &command)
    {
        TRY
            validate();
            GitService::Commit(_LogConfig.get(), _Workspace, command);
        CATCH
    }
    
    void GitManager::Amend()
    {
        TRY
            validate();
            GitService::Amend(_LogConfig.get(), _Workspace);
        CATCH
    }
    
}
