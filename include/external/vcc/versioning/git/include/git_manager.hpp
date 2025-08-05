#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "git_service.hpp"

namespace vcc
{
    class GitManager : public BaseManager
    {
        GETSET(std::wstring, Workspace, L"")
        VECTOR_SPTR(GitLog, GitLog)
       
        private:
            void validate() const;
        public:
            GitManager(std::shared_ptr<LogConfig> logConfig, const std::wstring &workspace = L"");
            ~GitManager() {}
            
            // General
            std::wstring getVersion() const;
            bool IsGitResponse() const;
            std::shared_ptr<GitStatus> getStatus(const GitStatusSearchCriteria *searchCriteria = nullptr) const;

            // Initialize
            void initializeGitResponse();
            void cloneGitResponse(const std::wstring &url, const GitCloneOption *option = nullptr);

            /*-----------------------------------*
            * ----------- Remote     -----------*
            * ----------------------------------*/
            // remote
            std::vector<std::shared_ptr<GitRemote>> getRemote();
            void AddRemote(const std::wstring &name, const std::wstring &url, const GitRemoteMirror &mirror = GitRemoteMirror::NA);
            void RenameRemote(const std::wstring &oldName, const std::wstring &newName);
            void RemoveRemote(const std::wstring &name);
            // fetch
            void FetchAll();
            // pull
            void Pull(const GitPullOption *option = nullptr);
            // push
            void Push(const GitPushOption *option = nullptr);

            /*-----------------------------------*
            * -----------  WorkTree  -----------*
            * ----------------------------------*/
            // TODO

            /*-----------------------------------*
            * -----------   Log      -----------*
            * ----------------------------------*/
            // To draw graph, mark the point by column index first, then link nodes if having same ParentHashID and HashID
            std::vector<std::shared_ptr<GitLog>> getLogs(const GitLogSearchCriteria *searchCriteria);
            
            /*-----------------------------------*
            * -----------    Tag     -----------*
            * ----------------------------------*/
            std::vector<std::wstring> getTags(const GitTagSearchCriteria *searchCriteria = nullptr);
            // Note: There is bug for Git, if using process, return string does not have branch and tags " (HEAD -> main, tag: v0.0.1)" after commit Hash ID. But it is normal if using terminal
            //void getTag(const std::wstring &tagName, std::shared_ptr<GitLog> log);
            std::shared_ptr<GitTagCurrentTag> getCurrentTag();
            void CreateTag(const std::wstring &tagName, const GitTagCreateTagOption *option = nullptr);
            void Switch(const std::wstring &tagName, bool isForce = false);
            void DeleteTag(const std::wstring &tagName);

            /*-----------------------------------*
            * -----------  Branch    -----------*
            * ----------------------------------*/
            std::wstring getCurrentBranchName();
            std::vector<std::shared_ptr<GitBranch>> getBranches();
            void CreateBranch( const std::wstring &branchName, const GitBranchCreateBranchOption *option = nullptr);
            void SwitchBranch(const std::wstring &branchName, const GitBranchSwitchBranchOption *option = nullptr);
            void RenameBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce = false);
            void CopyBranch(const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce = false);
            void DeleteBranch(const std::wstring &branchName, bool isForce = false);
            
            /*-----------------------------------*
            * ----------- Merge      -----------*
            * ----------------------------------*/
            // TODO

            /*-----------------------------------*
            * -----------Cherry Pick -----------*
            * ----------------------------------*/
            // TODO

            /*-----------------------------------*
            * ----------- Difference -----------*
            * ----------------------------------*/
            // Difference Summary only show different between commit and commit / commit and working files
            // hashIDs.size() == 0, then different from latest commit to current working files
            // hashIDs.size() == 1, then different from commit to current working files
            // hashIDs.size() > 1, then different between commit
            std::shared_ptr<GitDifferenceSummary> getDifferenceSummary(const std::vector<std::wstring> &hashIDs);
            std::shared_ptr<GitDifference> getDifferenceIndexFile(const GitDifferentSearchCriteria *searchCriteria = nullptr);
            std::shared_ptr<GitDifference> getDifferenceWorkingFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria = nullptr);
            std::shared_ptr<GitDifference> getDifferenceFile(const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria = nullptr);
            std::shared_ptr<GitDifference> getDifferenceCommit(const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria = nullptr);

            /*-----------------------------------*
            * ----------    Blame    -----------*
            * ----------------------------------*/
            // TODO

            /*-----------------------------------*
            * ----------- Stash      -----------*
            * ----------------------------------*/
            // TODO

            /*-----------------------------------*
            * ----------- Commit     -----------*
            * ----------------------------------*/
            void stage(const std::wstring &filePath);
            void stageAll();
            void unstage(const std::wstring &filePath);
            void unstageAll();
            void Commit(const std::wstring &command);
            void Amend();


        /*-----------------------------------*
         * ----------Reset/Restore-----------*
         * ----------------------------------*/
        // void DiscardFile(const std::wstring &filePath);
        // void RestoreFileToParticularCommit(const std::wstring &filePath, const std::wstring &hashID);
        // void ResetCommit(const GitResetMode &resetMode, const std::wstring &hashID);
        // void ReverseCommit(const std::wstring &hashID);
        
        // /*-----------------------------------*
        //  * ----------  Submodule   ----------*
        //  * ----------------------------------*/
        // // TODO

        // /*-----------------------------------*
        //  * ----------- Config     -----------*
        //  * ----------------------------------*/
        // // Overall Config
        // bool IsConfigExists(const std::wstring &key);
        // void getConfig(std::shared_ptr<GitConfig> config);
        // std::wstring getConfig(const std::wstring &key);
        // std::wstring getUserName();
        // std::wstring getUserEmail();
        
        // // Global Config
        // bool IsGlobalConfigExists(const LogConfig *logConfig, const std::wstring &key);
        // void getGlobalConfig(const LogConfig *logConfig, std::shared_ptr<GitConfig> config);
        // std::wstring getGlobalConfig(const LogConfig *logConfig, const std::wstring &key);
        // void setGlobalConfig(const LogConfig *logConfig, const std::wstring &key, const std::wstring &value);
        
        // std::wstring getGlobalUserName(const LogConfig *logConfig);
        // void setGlobalUserName(const LogConfig *logConfig, const std::wstring &value);
        // std::wstring getGlobalUserEmail(const LogConfig *logConfig);
        // void setGlobalUserEmail(const LogConfig *logConfig, const std::wstring &value);

        // // Local Config
        // bool IsLocalConfigExists(const std::wstring &key);
        // void getLocalConfig(std::shared_ptr<GitConfig> config);
        // std::wstring getLocalConfig(const std::wstring &key);
        // void setLocalConfig(const std::wstring &key, const std::wstring &value);

        // std::wstring getLocalUserName();
        // void setLocalUserName(const std::wstring &value);
        // std::wstring getLocalUserEmail();
        // void setLocalUserEmail(const std::wstring &value);
    };
}
