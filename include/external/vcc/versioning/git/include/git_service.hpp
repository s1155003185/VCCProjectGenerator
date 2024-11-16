#pragma once

#include <memory>

#ifdef _WIN32
#include <ctime>
#endif

#include "base_object.hpp"
#include "base_service.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    class GitStatusSearchCriteria : public BaseObject
    {
        GETSET(bool, IsWithIgnoreFiles, false);
        public:
            GitStatusSearchCriteria() : BaseObject() {}
            virtual ~GitStatusSearchCriteria() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitStatusSearchCriteria>(*this);
            }
    };
    
    class GitCloneOption : public BaseObject
    {
        GETSET(std::wstring, Branch, L"");
        GETSET(int64_t, Depth, -1);
        GETSET(bool, IsQuiet, false);
        
        public:
            GitCloneOption() : BaseObject() {}
            virtual ~GitCloneOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitCloneOption>(*this);
            }
    };

    enum class GitFileStatus
    {
        NA,
        Ignored,
        Untracked,
        Modified,
        TypeChanged,
        Added,
        Deleted,
        Renamed,
        Copied,
        UpdatedButUnmerged
    };

    class GitStatus : public BaseObject
    {
        GETSET(std::wstring, Branch, L"");
        GETSET(std::wstring, RemoteBranch, L"");

        MAP(GitFileStatus, std::vector<std::wstring>, IndexFiles);
        MAP(GitFileStatus, std::vector<std::wstring>, WorkingTreeFiles);

        public:
            GitStatus() : BaseObject() {}
            virtual ~GitStatus() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitStatus>(*this);
            }
    };

    enum class GitRemoteMirror
    {
        NA,
        Fetch,
        Push
    };

    class GitRemoteAddOption : public BaseObject
    {
        GETSET(std::wstring, Name, L"");
        GETSET(std::wstring, URL, L"");
        GETSET(GitRemoteMirror, Mirror, GitRemoteMirror::NA);
        public:
            GitRemoteAddOption() : BaseObject() {}
            virtual ~GitRemoteAddOption() {}
            
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitRemoteAddOption>(*this);
            }
    };

    class GitRemote : public BaseObject
    {
        GETSET(std::wstring, Name, L"");
        GETSET(std::wstring, URL, L"");
        GETSET(GitRemoteMirror, Mirror, GitRemoteMirror::Fetch);

        public:
            GitRemote() : BaseObject() {}
            virtual ~GitRemote() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitRemote>(*this);
            }
    };

    enum class GitPullOptionRecurseSubmodulesMode
    {
        Default,
        Yes,
        OnDemand,
        No
    };

    class GitPullOption : public BaseObject
    {
        GETSET(bool, IsQuite, false);
        GETSET(GitPullOptionRecurseSubmodulesMode, RecurseSubmodules, GitPullOptionRecurseSubmodulesMode::Default);

        VECTOR(std::wstring, Repositories);

        public:
            GitPullOption() : BaseObject() {}
            virtual ~GitPullOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitPullOption>(*this);
            }
    };

    class GitPushOption : public BaseObject
    {
        GETSET(bool, IsForce, false);
        GETSET(bool, IsQuite, false);

        VECTOR(std::wstring, Repositories);

        public:
            GitPushOption() : BaseObject() {}
            virtual ~GitPushOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitPushOption>(*this);
            }
    };

    enum class GitLogOrderBy
    {
        NA,
        Date,
        AuthorDate,
        Topo,
        Reverse
    };

    class GitLogSearchCriteria : public BaseObject
    {
        // Commit Limiting
        GETSET(int64_t, LogCount, -1);
        GETSET(int64_t, Skip, -1);
        GETSET(std::wstring, DateAfter, L"");
        GETSET(std::wstring, DateBefore, L"");
        GETSET(std::wstring, Author, L"");
        GETSET(std::wstring, Committer, L"");
        GETSET(std::wstring, Grep, L"");
        GETSET(bool, IsGrepAllMatch, false);
        GETSET(bool, IsGrepInvertGrep, false);
        GETSET(bool, IsPatternRegexpIgnoreCase, false);
        GETSET(bool, IsPatternExtendedRegexp, false);
        GETSET(bool, IsPatternFixedStrings, false);
        GETSET(bool, IsMerges, false);
        GETSET(bool, IsNoMerges, false);
        GETSET(int64_t, MinParents, -1);
        GETSET(int64_t, MaxParents, -1);
        GETSET(bool, IsFirstParent, false);
        GETSET(bool, IsAll, false);
        GETSET(bool, IsAllBranches, false);
        GETSET(std::wstring, Branches, L"");
        GETSET(bool, IsAllTags, false);
        GETSET(std::wstring, Tags, L"");
        GETSET(bool, IsAllRemotes, false);
        GETSET(std::wstring, Remotes, L"");
        GETSET(bool, IsAllGlob, false);
        GETSET(std::wstring, Glob, L"");
        GETSET(std::wstring, ExcludeGlob, L"");

        // History Simplication
        GETSET(bool, IsSimplifyByDecoration, false);
        GETSET(bool, IsShowPulls, false);
        GETSET(bool, IsFullHistory, false);
        GETSET(bool, IsSparse, false);
        GETSET(bool, IsSimplifyMerges, false);
        GETSET(std::wstring, AncestryPath, L"");

        // Order By
        GETSET(GitLogOrderBy, OrderBy, GitLogOrderBy::NA);

        // Revision Range
        GETSET(std::wstring, RevisionFrom, L"");
        GETSET(std::wstring, RevisionTo, L"");

        // Paths
        VECTOR(std::wstring, Paths);

        public:
            GitLogSearchCriteria() : BaseObject() {}
            virtual ~GitLogSearchCriteria() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitLogSearchCriteria>(*this);
            }
    };

    class GitLog : public BaseObject
    {
        GETSET(int64_t, ColumnIndex, (size_t)0);
        GETSET(std::wstring, HashID, L"");
        GETSET(std::wstring, AbbreviatedHashID, L"");
        GETSET(std::wstring, TreeHashID, L"");
        GETSET(std::wstring, AbbreviatedTreeHashID, L"");
        VECTOR(std::wstring, ParentHashIDs);
        VECTOR(std::wstring, AbbreviatedParentHashIDs);
        GETSET(bool, IsHead, false);
        VECTOR(std::wstring, Branches);
        VECTOR(std::wstring, Tags);
        GETSET(std::wstring, Author, L"");
        GETSET(std::wstring, AuthorEmail, L"");
        GETSET(time_t, AuthorDate, -1);
        GETSET(std::wstring, AuthorDateStr, L"");
        GETSET(std::wstring, Committer, L"");
        GETSET(std::wstring, CommitterEmail, L"");
        GETSET(time_t, CommitDate, -1);
        GETSET(std::wstring, CommitDateStr, L"");
        GETSET(std::wstring, Title, L"");
        GETSET(std::wstring, Message, L"");
        GETSET(std::wstring, FullMessage, L"");

        public:
            GitLog() : BaseObject() {}
            virtual ~GitLog() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitLog>(*this);
            }
    };

    class GitTagSearchCriteria : public BaseObject
    {
        GETSET(std::wstring, Contains, L"");
        GETSET(std::wstring, NoContains, L"");

        GETSET(std::wstring, OrderBy, L"");

        public:
            GitTagSearchCriteria() : BaseObject() {}
            virtual ~GitTagSearchCriteria() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitTagSearchCriteria>(*this);
            }
    };

    class GitTagCurrentTag : public BaseObject
    {
        GETSET(std::wstring, TagName, L"");
        GETSET(int64_t, NoOfCommit, 0);
        GETSET(std::wstring, HashID, L"");

        public:
            GitTagCurrentTag() : BaseObject() {}
            virtual ~GitTagCurrentTag() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitTagCurrentTag>(*this);
            }
    };

    enum class GitTagCreateTagOptionSignMode 
    {
        Default,
        NoSign,
        Sign,
        LocalUser
    };

    class GitTagCreateTagOption : public BaseObject
    {
        GETSET(GitTagCreateTagOptionSignMode, Sign, GitTagCreateTagOptionSignMode::Default);
        GETSET(std::wstring, SignLocalUserKeyID, L"");
        GETSET(bool, IsForce, false;);
        GETSET(std::wstring, Message, L"");

        GETSET(std::wstring, HashID, L"");

        public:
            GitTagCreateTagOption() : BaseObject() {}
            virtual ~GitTagCreateTagOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitTagCreateTagOption>(*this);
            }
    };

    class GitBranch : public BaseObject
    {
        GETSET(std::wstring, Name, L"");
        GETSET(bool, IsActive, false);
        GETSET(std::wstring, HashID, L"");
        GETSET(std::wstring, Title, L"");
        GETSET(std::wstring, PointToBranch, L"");

        public:
            GitBranch() : BaseObject() {}
            virtual ~GitBranch() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitBranch>(*this);
            }
    };

    enum class GitBranchCreateBranchOptionTrackMode {
        Default,
        No,
        Direct,
        Inherit
    };

    class GitBranchCreateBranchOption : public BaseObject
    {
        GETSET(bool, IsForce, false);
        GETSET(GitBranchCreateBranchOptionTrackMode, Track, GitBranchCreateBranchOptionTrackMode::Default);
        GETSET(bool, IsRecurseSubmodules, false);

        public:
            GitBranchCreateBranchOption() : BaseObject() {}
            virtual ~GitBranchCreateBranchOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitBranchCreateBranchOption>(*this);
            }
    };

    class GitBranchSwitchBranchOption : public BaseObject
    {
        GETSET(bool, IsQuite, false);
        GETSET(bool, IsDiscardChanges, false);
        public:
            GitBranchSwitchBranchOption() : BaseObject() {}
            virtual ~GitBranchSwitchBranchOption() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitBranchSwitchBranchOption>(*this);
            }
    };

    class GitDifferentSearchCriteria : public BaseObject
    {
        GETSET(int64_t, NoOfLines, -1);
        VECTOR(std::wstring, HashIDs);

        public:
            GitDifferentSearchCriteria() : BaseObject() {}
            virtual ~GitDifferentSearchCriteria() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitDifferentSearchCriteria>(*this);
            }
    };

    class GitDifferenceSummary : public BaseObject
    {
        VECTOR(std::wstring, Files);
        VECTOR(size_t, AddLineCounts);
        VECTOR(size_t, DeleteLineCounts);

        public:
            GitDifferenceSummary() : BaseObject() {}
            virtual ~GitDifferenceSummary() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitDifferenceSummary>(*this);
            }
    };

    class GitDifference : public BaseObject
    {
        GETSET(std::wstring, FilePathOld, L"");
        GETSET(std::wstring, FilePathNew, L"");
        VECTOR(size_t, LineNumberOld);
        VECTOR(size_t, LineCountOld);
        VECTOR(size_t, LineNumberNew);
        VECTOR(size_t, LineCountNew);
        VECTOR(std::wstring, ChangedLines);

        public:
            GitDifference() : BaseObject() {}
            virtual ~GitDifference() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<GitDifference>(*this);
            }
    };

    enum class GitResetMode
    {
        NA,
        Soft,
        Mixed,
        Hard,
        Merge,
        Keep
    };

    class GitConfig : public BaseObject
    {
        GETSET(std::wstring, UserName, L"");
        GETSET(std::wstring, UserEmail, L"");
        MAP(std::wstring, std::wstring, Configs);

        public:
            GitConfig() : BaseObject() {}
            virtual ~GitConfig() {}

            virtual std::shared_ptr<IObject> Clone() const override
            {
                std::shared_ptr<GitConfig> obj = std::make_shared<GitConfig>(*this);
                obj->CloneConfigs(this->GetConfigs());
                return obj;
            }
    };

    class GitService : public BaseService
    {
        private:
            static GitFileStatus ParseGitFileStatus(const wchar_t &c);

        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(const LogConfig *logConfig, const std::wstring &command);

        // General
        static std::wstring GetVersion(const LogConfig *logConfig);
        static bool IsGitResponse(const LogConfig *logConfig, const std::wstring &workspace);
        static void GetStatus(const LogConfig *logConfig, const std::wstring &workspace, const GitStatusSearchCriteria *searchCriteria, std::shared_ptr<GitStatus> status);

        // Initialization
        static void Initialize(const LogConfig *logConfig, const std::wstring &workspace);
        static void Clone(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &url, const GitCloneOption *option = nullptr);

        /*-----------------------------------*
         * ----------- Remote     -----------*
         * ----------------------------------*/
        // remote
        static void GetRemote(const LogConfig *logConfig, const std::wstring &workspace, std::vector<std::shared_ptr<GitRemote>> &remotes);
        static void AddRemote(const LogConfig *logConfig, const std::wstring &workspace, const GitRemoteAddOption *option = nullptr);
        static void RenameRemote(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldName, const std::wstring &newName);
        static void RemoveRemote(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &name);
        // fetch
        static void FetchAll(const LogConfig *logConfig, const std::wstring &workspace);
        // pull
        static void Pull(const LogConfig *logConfig, const std::wstring &workspace, const GitPullOption *option = nullptr);
        // push
        static void Push(const LogConfig *logConfig, const std::wstring &workspace, const GitPushOption *option = nullptr);

        /*-----------------------------------*
         * -----------  WorkTree  -----------*
         * ----------------------------------*/
        // TODO

        /*-----------------------------------*
         * -----------   Log      -----------*
         * ----------------------------------*/
        static std::wstring GetGitLogSearchCriteriaString(const GitLogSearchCriteria *searchCriteria);
        // only parse git log --graph --oneline --pretty=format:"(%H)(%h)(%T)(%t)(%P)(%p)"
        static void ParseGitLogGraph(const std::wstring &str, std::vector<std::shared_ptr<GitLog>> &logs);
        // only parse pattern L"Thu Jan 25 22:47:35 2024 +0800"
        static std::time_t ParseGitLogDatetime(const std::wstring &datimeStr);
        static void ParseGitLog(const std::wstring &str, std::shared_ptr<GitLog> log);
        // To draw graph, mark the point by column index first, then link nodes if having same ParentHashID and HashID
        static void GetLogs(const LogConfig *logConfig, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria, std::vector<std::shared_ptr<GitLog>> &logs);
        static void GetCurrentLog(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitLog> log);
        // Get log by GetLogs first, then put the share pointer to GetLogDetail
        static void GetLog(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &hashID, std::shared_ptr<GitLog> log);

        /*-----------------------------------*
         * -----------    Tag     -----------*
         * ----------------------------------*/
        static void GetTags(const LogConfig *logConfig, const std::wstring &workspace, const GitTagSearchCriteria *searchCriteria, std::vector<std::wstring> &tags);
        // Note: There is bug for Git, if using process, return string does not have branch and tags " (HEAD -> main, tag: v0.0.1)" after commit Hash ID. But it is normal if using terminal
        //static void GetTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, std::shared_ptr<GitLog> log);
        static GitTagCurrentTag GetCurrentTag(const LogConfig *logConfig, const std::wstring &workspace);
        static void CreateTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, const GitTagCreateTagOption *option = nullptr);
        // Window behavior and Linux Behavior different, Window throw exception (tag will detach branch) while Linux will not
        // Can use GitService::SwitchReverse to switch back
        static void Switch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, bool isForce = false);
        // Undo Switch
        static void SwitchReverse(const LogConfig *logConfig, const std::wstring &workspace);
        static void DeleteTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName);

        /*-----------------------------------*
         * -----------  Branch    -----------*
         * ----------------------------------*/
        // only for 
        // 1. L"* master hashID Title"
        // 2. L"  master hashID Title"
        // 3. L"  head -> orgin/master"
        static void ParseGitBranch(const std::wstring &str, std::shared_ptr<GitBranch> branch);
        // static void GetCurrentBranch(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitBranch> branch);
        static std::wstring GetCurrentBranchName(const LogConfig *logConfig, const std::wstring &workspace);
        static void GetBranches(const LogConfig *logConfig, const std::wstring &workspace, std::vector<std::shared_ptr<GitBranch>> &branches);
        static void CreateBranch(const LogConfig *logConfig, const std::wstring &workspace,  const std::wstring &branchName, const GitBranchCreateBranchOption *option = nullptr);
        static void SwitchBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &branchName, const GitBranchSwitchBranchOption *option = nullptr);
        static void RenameBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce = false);
        static void CopyBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce = false);
        static void DeleteBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &branchName, bool isForce = false);
        
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
        static void GetDifferenceSummary(const LogConfig *logConfig, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, std::shared_ptr<GitDifferenceSummary> summary);
        // filePath must be filled
        static void ParseGitDiff(const std::wstring &str, std::shared_ptr<GitDifference> difference);
        static void GetDifferenceIndexFile(const LogConfig *logConfig, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath,  std::shared_ptr<GitDifference> diff);
        static void GetDifferenceWorkingFile(const LogConfig *logConfig, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath, std::shared_ptr<GitDifference> diff);
        static void GetDifferenceFile(const LogConfig *logConfig, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath, std::shared_ptr<GitDifference> diff);
        static void GetDifferenceCommit(const LogConfig *logConfig, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, std::shared_ptr<GitDifference> diff);

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
        static void Stage(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath);
        static void StageAll(const LogConfig *logConfig, const std::wstring &workspace);
        static void Unstage(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath);
        static void UnstageAll(const LogConfig *logConfig, const std::wstring &workspace);
        static void Commit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &command);
        static void Amend(const LogConfig *logConfig, const std::wstring &workspace);


        /*-----------------------------------*
         * ----------Reset/Restore-----------*
         * ----------------------------------*/
        static void DiscardFile(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath);
        static void RestoreFileToParticularCommit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath, const std::wstring &hashID);
        static void ResetCommit(const LogConfig *logConfig, const std::wstring &workspace, const GitResetMode &resetMode, const std::wstring &hashID);
        static void ReverseCommit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &hashID);
        
        /*-----------------------------------*
         * ----------  Submodule   ----------*
         * ----------------------------------*/
        // TODO

        /*-----------------------------------*
         * ----------- Config     -----------*
         * ----------------------------------*/
        // Overall Config
        static bool IsConfigExists(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key);
        static void GetConfig(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key);
        static std::wstring GetUserName(const LogConfig *logConfig, const std::wstring &workspace);
        static std::wstring GetUserEmail(const LogConfig *logConfig, const std::wstring &workspace);
        
        // Global Config
        static bool IsGlobalConfigExists(const LogConfig *logConfig, const std::wstring &key);
        static void GetGlobalConfig(const LogConfig *logConfig, std::shared_ptr<GitConfig> config);
        static std::wstring GetGlobalConfig(const LogConfig *logConfig, const std::wstring &key);
        static void SetGlobalConfig(const LogConfig *logConfig, const std::wstring &key, const std::wstring &value);
        
        static std::wstring GetGlobalUserName(const LogConfig *logConfig);
        static void SetGlobalUserName(const LogConfig *logConfig, const std::wstring &value);
        static std::wstring GetGlobalUserEmail(const LogConfig *logConfig);
        static void SetGlobalUserEmail(const LogConfig *logConfig, const std::wstring &value);

        // Local Config
        static bool IsLocalConfigExists(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key);
        static void GetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key);
        static void SetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key, const std::wstring &value);

        static std::wstring GetLocalUserName(const LogConfig *logConfig, const std::wstring &workspace);
        static void SetLocalUserName(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &value);
        static std::wstring GetLocalUserEmail(const LogConfig *logConfig, const std::wstring &workspace);
        static void SetLocalUserEmail(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &value);
    };
}
