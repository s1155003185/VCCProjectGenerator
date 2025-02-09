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

    void GitManager::Validate() const
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
            Validate();
            return GitService::IsGitResponse(_LogConfig.get(), _Workspace);
        CATCH
        return false;
    }

    std::shared_ptr<GitStatus> GitManager::GetStatus(const GitStatusSearchCriteria *searchCriteria) const
    {
        TRY
            Validate();
            return GitService::GetStatus(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }

    void GitManager::InitializeGitResponse()
    {
        TRY
            Validate();
            GitService::InitializeGitResponse(_LogConfig.get(), _Workspace);
        CATCH
    }

    void GitManager::CloneGitResponse(const std::wstring &url, const GitCloneOption *option)
    {
        TRY
            Validate();
            GitService::CloneGitResponse(_LogConfig.get(), _Workspace, url, option);
        CATCH
    }

    std::vector<std::shared_ptr<GitRemote>> GitManager::GetRemote()
    {
        TRY
            Validate();
            return GitService::GetRemote(_LogConfig.get(), _Workspace);
        CATCH
        return {};
    }

    void GitManager::AddRemote(const std::wstring &name, const std::wstring &url, const GitRemoteMirror &mirror)
    {
        TRY
            Validate();
            GitService::AddRemote(_LogConfig.get(), _Workspace, name, url, mirror);
        CATCH
    }
    
    void GitManager::RenameRemote(const std::wstring &oldName, const std::wstring &newName)
    {
        TRY
            Validate();
            GitService::RenameRemote(_LogConfig.get(), _Workspace, oldName, newName);
        CATCH
    }
    
    void GitManager::RemoveRemote(const std::wstring &name)
    {
        TRY
            Validate();
            GitService::RemoveRemote(_LogConfig.get(), _Workspace, name);
        CATCH
    }
    
    void GitManager::FetchAll()
    {
        TRY
            Validate();
            GitService::FetchAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Pull(const GitPullOption *option)
    {
        TRY
            Validate();
            GitService::Pull(_LogConfig.get(), _Workspace, option);
        CATCH
    }
    
    void GitManager::Push(const GitPushOption *option)
    {
        TRY
            Validate();
            GitService::Push(_LogConfig.get(), _Workspace, option);
        CATCH
    }
    
    std::vector<std::shared_ptr<GitLog>> GitManager::GetLogs(const GitLogSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetLogs(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::vector<std::wstring> GitManager::GetTags(const GitTagSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetTags(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::shared_ptr<GitTagCurrentTag> GitManager::GetCurrentTag()
    {
        TRY
            Validate();
            return GitService::GetCurrentTag(_LogConfig.get(), _Workspace);
        CATCH
        return nullptr;
    }
    
    void GitManager::CreateTag(const std::wstring &tagName, const GitTagCreateTagOption *option)
    {
        TRY
            Validate();
            GitService::CreateTag(_LogConfig.get(), _Workspace, tagName, option);
        CATCH
    }
    
    void GitManager::Switch(const std::wstring &tagName, bool isForce)
    {
        TRY
            Validate();
            GitService::Switch(_LogConfig.get(), _Workspace, tagName, isForce);
        CATCH
    }
    
    void GitManager::DeleteTag(const std::wstring &tagName)
    {
        TRY
            Validate();
            GitService::DeleteTag(_LogConfig.get(), _Workspace, tagName);
        CATCH
    }
    
    std::wstring GitManager::GetCurrentBranchName()
    {
        TRY
            Validate();
            return GitService::GetCurrentBranchName(_LogConfig.get(), _Workspace);
        CATCH
        return L"";
    }
    
    std::vector<std::shared_ptr<GitBranch>> GitManager::GetBranches()
    {
        TRY
            Validate();
            return GitService::GetBranches(_LogConfig.get(), _Workspace);
        CATCH
        return {};
    }
    
    void GitManager::CreateBranch( const std::wstring &branchName, const GitBranchCreateBranchOption *option)
    {
        TRY
            Validate();
            GitService::CreateBranch(_LogConfig.get(), _Workspace, branchName, option);
        CATCH
    }
    
    void GitManager::SwitchBranch(const std::wstring &branchName, const GitBranchSwitchBranchOption *option)
    {
        TRY
            Validate();
            GitService::SwitchBranch(_LogConfig.get(), _Workspace, branchName, option);
        CATCH
    }
    
    void GitManager::RenameBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            Validate();
            GitService::RenameBranch(_LogConfig.get(), _Workspace, oldBranchName, newBranchName, isForce);
        CATCH
    }
    
    void GitManager::CopyBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            Validate();
            GitService::CopyBranch(_LogConfig.get(), _Workspace, oldBranchName, newBranchName, isForce);
        CATCH
    }
    
    void GitManager::DeleteBranch(const std::wstring &branchName, bool isForce)
    {
        TRY
            Validate();
            GitService::DeleteBranch(_LogConfig.get(), _Workspace, branchName, isForce);
        CATCH
    }
    
    std::shared_ptr<GitDifferenceSummary> GitManager::GetDifferenceSummary(const std::vector<std::wstring> &hashIDs)
    {
        TRY
            Validate();
            return GitService::GetDifferenceSummary(_LogConfig.get(), _Workspace, hashIDs);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceIndexFile(const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetDifferenceIndexFile(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceWorkingFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetDifferenceWorkingFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::GetDifferenceFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetDifferenceFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitManager::GetDifferenceCommit(const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            Validate();
            return GitService::GetDifferenceCommit(_LogConfig.get(), _Workspace, fromHashID, toHashID, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    void GitManager::Stage(const std::wstring &filePath)
    {
        TRY
            Validate();
            GitService::Stage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::StageAll()
    {
        TRY
            Validate();
            GitService::StageAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Unstage(const std::wstring &filePath)
    {
        TRY
            Validate();
            GitService::Unstage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::UnstageAll()
    {
        TRY
            Validate();
            GitService::UnstageAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::Commit(const std::wstring &command)
    {
        TRY
            Validate();
            GitService::Commit(_LogConfig.get(), _Workspace, command);
        CATCH
    }
    
    void GitManager::Amend()
    {
        TRY
            Validate();
            GitService::Amend(_LogConfig.get(), _Workspace);
        CATCH
    }
    
}
