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
            VALIDATE(L"Workspace is empty", !isBlank(_Workspace))
        CATCH
    }

    std::wstring GitManager::getVersion() const
    {
        TRY
            return GitService::getVersion(_LogConfig.get());
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

    std::shared_ptr<GitStatus> GitManager::getStatus(const GitStatusSearchCriteria *searchCriteria) const
    {
        TRY
            validate();
            return GitService::getStatus(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }

    void GitManager::initializeGitResponse()
    {
        TRY
            validate();
            GitService::initializeGitResponse(_LogConfig.get(), _Workspace);
        CATCH
    }

    void GitManager::cloneGitResponse(const std::wstring &url, const GitCloneOption *option)
    {
        TRY
            validate();
            GitService::cloneGitResponse(_LogConfig.get(), _Workspace, url, option);
        CATCH
    }

    std::vector<std::shared_ptr<GitRemote>> GitManager::getRemote()
    {
        TRY
            validate();
            return GitService::getRemote(_LogConfig.get(), _Workspace);
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
    
    std::vector<std::shared_ptr<GitLog>> GitManager::getLogs(const GitLogSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getLogs(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::vector<std::wstring> GitManager::getTags(const GitTagSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getTags(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return {};
    }
    
    std::shared_ptr<GitTagCurrentTag> GitManager::getCurrentTag()
    {
        TRY
            validate();
            return GitService::getCurrentTag(_LogConfig.get(), _Workspace);
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
    
    std::wstring GitManager::getCurrentBranchName()
    {
        TRY
            validate();
            return GitService::getCurrentBranchName(_LogConfig.get(), _Workspace);
        CATCH
        return L"";
    }
    
    std::vector<std::shared_ptr<GitBranch>> GitManager::getBranches()
    {
        TRY
            validate();
            return GitService::getBranches(_LogConfig.get(), _Workspace);
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
    
    std::shared_ptr<GitDifferenceSummary> GitManager::getDifferenceSummary(const std::vector<std::wstring> &hashIDs)
    {
        TRY
            validate();
            return GitService::getDifferenceSummary(_LogConfig.get(), _Workspace, hashIDs);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::getDifferenceIndexFile(const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getDifferenceIndexFile(_LogConfig.get(), _Workspace, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::getDifferenceWorkingFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getDifferenceWorkingFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    std::shared_ptr<GitDifference> GitManager::getDifferenceFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getDifferenceFile(_LogConfig.get(), _Workspace, filePath, searchCriteria);
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitManager::getDifferenceCommit(const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            validate();
            return GitService::getDifferenceCommit(_LogConfig.get(), _Workspace, fromHashID, toHashID, filePath, searchCriteria);
        CATCH
        return nullptr;
    }
    
    void GitManager::stage(const std::wstring &filePath)
    {
        TRY
            validate();
            GitService::stage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::stageAll()
    {
        TRY
            validate();
            GitService::stageAll(_LogConfig.get(), _Workspace);
        CATCH
    }
    
    void GitManager::unstage(const std::wstring &filePath)
    {
        TRY
            validate();
            GitService::unstage(_LogConfig.get(), _Workspace, filePath);
        CATCH
    }
    
    void GitManager::unstageAll()
    {
        TRY
            validate();
            GitService::unstageAll(_LogConfig.get(), _Workspace);
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
