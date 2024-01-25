#pragma once
#include "base_service.hpp"

#include <memory>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"

namespace vcc
{
    constexpr auto GIT_LOG_ID = L"GIT";

    // CheckSum

    class GitConfig : public BaseObject {
        GETSET(std::wstring, UserName, L"");
        GETSET(std::wstring, UserEmail, L"");
        MAP(std::wstring, std::wstring, Configs);

        public:
            GitConfig() : BaseObject() {}
            virtual ~GitConfig() {}

            virtual std::shared_ptr<IObject> Clone() override {
                std::shared_ptr<GitConfig> obj = std::make_shared<GitConfig>(*this);
                obj->CloneConfigs(this->GetConfigs());
                return obj;
            }
    };

    enum class GitFileStatus {
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

    class GitStatus : public BaseObject {
        GETSET(std::wstring, Branch, L"");
        GETSET(std::wstring, RemoteBranch, L"");

        MAP(GitFileStatus, std::vector<std::wstring>, IndexFiles);
        MAP(GitFileStatus, std::vector<std::wstring>, WorkingTreeFiles);

        public:
            GitStatus() : BaseObject() {}
            virtual ~GitStatus() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitStatus>(*this);
            }
    };

    enum class GitLogOrderBy {
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

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitLogSearchCriteria>(*this);
            }
    };

    class GitLog : public BaseObject {
        GETSET(size_t, ColumnIndex, (size_t)0);
        GETSET(std::wstring, HashID, L"");
        GETSET(std::wstring, AbbreviatedHashID, L"");
        GETSET(std::wstring, TreeHashID, L"");
        GETSET(std::wstring, AbbreviatedTreeHashID, L"");
        VECTOR(std::wstring, ParentHashIDs);
        VECTOR(std::wstring, AbbreviatedParentHashIDs);
        GETSET(bool, IsHead, false);
        VECTOR(std::wstring, Tags);
        GETSET(std::wstring, Author, L"");
        GETSET(std::wstring, AuthorEmail, L"");
        GETSET(time_t, AuthorDate, -1);
        GETSET(std::wstring, AuthorDateStr, L"");
        GETSET(std::wstring, Committer, L"");
        GETSET(std::wstring, CommitterEmail, L"");
        GETSET(time_t, CommitterDate, -1);
        GETSET(std::wstring, CommitterDateStr, L"");
        GETSET(std::wstring, Title, L"");
        GETSET(std::wstring, Message, L"");

        public:
            GitLog() : BaseObject() {}
            virtual ~GitLog() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitLog>(*this);
            }
    };

    class GitDifferenceSummary : public BaseObject {
        friend class GitService;

        VECTOR(std::wstring, Files);
        VECTOR(size_t, AddLineCounts);
        VECTOR(size_t, DeleteLineCounts);

        public:
            GitDifferenceSummary() : BaseObject() {}
            virtual ~GitDifferenceSummary() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitDifferenceSummary>(*this);
            }
    };

    class GitDifference : public BaseObject {
        friend class GitService;

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

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<GitDifference>(*this);
            }
    };

    enum class GitResetMode {
        NA,
        Soft,
        Mixed,
        Hard,
        Merge,
        Keep
    };

    class GitService : public BaseService
    {
        private:
            static GitFileStatus ParseGitFileStatus(const wchar_t &c);

        public:
            GitService() : BaseService() {}
            ~GitService() {}

        static std::wstring Execute(const LogProperty *logProperty, const std::wstring &command);

        // General
        static std::wstring GetVersion(const LogProperty *logProperty);
        static bool IsGitResponse(const LogProperty *logProperty, const std::wstring &workspace);
        static void GetStatus(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitStatus> status, bool isWithIgnoredFiles = false);
        static void Pull(const LogProperty *logProperty, const std::wstring &workspace);
    
        // Initialization
        static void Initialize(const LogProperty *logProperty, const std::wstring &workspace);
        static void Clone(const LogProperty *logProperty, const std::wstring &url, const std::wstring &branch, const std::wstring &dist, const int64_t &depth = -1);
        static void CheckOut(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branch);

        // Difference
        // Difference Summary only show different between commit and commit / commit and working files
        // hashIDs.size() == 0, then different from latest commit to current working files
        // hashIDs.size() == 1, then different from commit to current working files
        // hashIDs.size() > 1, then different between commit
        static void GetDifferenceSummary(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, std::shared_ptr<GitDifferenceSummary> summary);
        // filePath must be filled
        static void ParseGitDiff(const std::wstring &str, std::shared_ptr<GitDifference> difference);
        static void GetDifferenceIndexFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine = -1);
        static void GetDifferenceWorkingFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine = -1);
        static void GetDifferenceFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine = -1);
        static void GetDifferenceCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine = -1);

        // Log
        static std::wstring GetGitLogSearchCriteriaString(const GitLogSearchCriteria *searchCriteria);
        // only parse git log --graph --oneline --pretty=format:"(%H)(%h)(%T)(%t)(%P)(%p)"
        static void ParseGitLogGraph(const std::wstring &str, std::vector<std::shared_ptr<GitLog>> &logs);
        static void ParseGitLog(const std::wstring &str, std::shared_ptr<GitLog> &log);
        // To draw graph, mark the point by column index first, then link nodes if having same ParentHashID and HashID
        static void GetLogs(const LogProperty *logProperty, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria, std::vector<std::shared_ptr<GitLog>> &logs);
        // Get log by GetLogs first, then put the share pointer to GetLogDetail
        static void GetLogDetail(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitLog> log);

        // Commit
        static void Stage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath);
        static void StageAll(const LogProperty *logProperty, const std::wstring &workspace);
        static void Unstage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath);
        static void UnstageAll(const LogProperty *logProperty, const std::wstring &workspace);
        static void Commit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &command);

        // Reset / Restore
        static void DiscardFile(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath);
        static void RestoreFileToParticularCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath, const std::wstring &hashID);
        static void ResetCommit(const LogProperty *logProperty, const std::wstring &workspace, const GitResetMode &resetMode, const std::wstring &hashID);
        static void ReverseCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &hashID);
        
        /*----------------------------------*
        * ----------- Config     -----------*
        * ----------------------------------*/
        // Overall Config
        static bool IsConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static std::wstring GetUserName(const LogProperty *logProperty, const std::wstring &workspace);
        static std::wstring GetUserEmail(const LogProperty *logProperty, const std::wstring &workspace);
        
        // Global Config
        static bool IsGlobalConfigExists(const LogProperty *logProperty, const std::wstring &key);
        static void GetGlobalConfig(const LogProperty *logProperty, std::shared_ptr<GitConfig> config);
        static std::wstring GetGlobalConfig(const LogProperty *logProperty, const std::wstring &key);
        static void SetGlobalConfig(const LogProperty *logProperty, const std::wstring &key, const std::wstring &value);
        
        static std::wstring GetGlobalUserName(const LogProperty *logProperty);
        static void SetGlobalUserName(const LogProperty *logProperty, const std::wstring &value);
        static std::wstring GetGlobalUserEmail(const LogProperty *logProperty);
        static void SetGlobalUserEmail(const LogProperty *logProperty, const std::wstring &value);

        // Local Config
        static bool IsLocalConfigExists(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitConfig> config);
        static std::wstring GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key);
        static void SetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value);

        static std::wstring GetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace);
        static void SetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value);
        static std::wstring GetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace);
        static void SetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value);
    };
}
