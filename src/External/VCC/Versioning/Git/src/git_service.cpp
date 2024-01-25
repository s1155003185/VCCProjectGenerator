#include "git_service.hpp"

#include <assert.h>
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
#include "vector_helper.hpp"

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
                    return GitFileStatus::Modified;
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
                case L'U':
                    return GitFileStatus::UpdatedButUnmerged;
                default:
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unknown Git file status " + std::wstring(1, c));
            }
        )
        return GitFileStatus::NA;
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

    void GitService::GetDifferenceSummary(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, std::shared_ptr<GitDifferenceSummary> summary)
    {
        TRY_CATCH(
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff " + Concat(hashIDs, L" ") + L" --numstat"));
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, L"\t", { L"\"" }, { L"\"" }, { L"\\" });
                if (tokens.size() < 3)
                    continue;
                summary->InsertFiles(tokens.at(2));
                summary->InsertAddLineCounts((size_t)stoi(tokens.at(0)));
                summary->InsertDeleteLineCounts((size_t)stoi(tokens.at(1)));
            }
        )
    }

    void GitService::ParseGitDiff(const std::wstring &str, std::shared_ptr<GitDifference> difference)
    {
        TRY_CATCH(
            std::wstring filePathOldPrefix = L"--- a/";
            std::wstring filePathNewPrefix = L"+++ b/";
            std::wstring lineCountPrefix = L"@@";
            std::vector<std::wstring> lines = SplitStringByLine(str);
            size_t rowCount = 0;
            size_t currentItem = 0;
            std::wstring currentText = L"";
            for (const std::wstring &line : lines) {
                if (rowCount > 0) {
                    currentText += line + L"\n";
                    if (rowCount == 1) {
                        difference->InsertChangedLines(currentText);
                        currentText = L"";
                    }
                    rowCount--;
                } else {
                    if (HasPrefix(line, filePathOldPrefix)) {
                        std::wstring tmpStr = line.substr(filePathOldPrefix.length());
                        Trim(tmpStr);
                        difference->SetFilePathOld(tmpStr);
                    } else if (HasPrefix(line, filePathNewPrefix)){
                        std::wstring tmpStr = line.substr(filePathNewPrefix.length());
                        Trim(tmpStr);
                        difference->SetFilePathNew(tmpStr);
                    } else if (HasPrefix(line, lineCountPrefix)) {
                        // sample new: @@ -1 +1,2 @@
                        // sample modify: @@ -116,10 +116,10 @@ xxxx
                        // 1. Chop to remove all @@
                        // 2. Get all numbers
                        std::wstring tmpStr = line.substr(lineCountPrefix.length());
                        tmpStr = tmpStr.substr(0, tmpStr.find(lineCountPrefix));
                        Trim(tmpStr);
                        std::vector<std::wstring> tokens = SplitString(tmpStr, L" ");
                        if (tokens.size() != 2)
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        // old
                        std::vector<std::wstring> lineCountOld = SplitString(tokens[0], L",");
                        if (!(lineCountOld.size() == 1 || lineCountOld.size() == 2))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        std::wstring lineNumberStr = lineCountOld[0];
                        Trim(lineNumberStr);
                        if (!(HasPrefix(lineNumberStr, L"-") && lineNumberStr.length() > 1))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        difference->InsertLineNumberOld((size_t)stoi(lineNumberStr.substr(1)));
                        if (lineCountOld.size() > 1) {
                            std::wstring lineCountStr = lineCountOld[1];
                            Trim(lineCountStr);
                            difference->InsertLineCountOld((size_t)stoi(lineCountStr));
                        } else
                            difference->InsertLineCountOld((size_t)0);

                        // new
                        std::vector<std::wstring> lineCountNew = SplitString(tokens[1], L",");
                        if (!(lineCountNew.size() == 1 || lineCountNew.size() == 2))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        lineNumberStr = lineCountNew[0];
                        Trim(lineNumberStr);
                        if (!(HasPrefix(lineNumberStr, L"+") && lineNumberStr.length() > 1))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        difference->InsertLineNumberNew((size_t)stoi(lineNumberStr.substr(1)));
                        if (lineCountNew.size() > 1) {
                            std::wstring lineCountStr = lineCountNew[1];
                            Trim(lineCountStr);
                            difference->InsertLineCountNew((size_t)stoi(lineCountStr));
                        } else
                            difference->InsertLineCountNew((size_t)0);

                        currentItem = difference->GetLineCountNew().size() - 1;
                        rowCount = std::max(difference->GetLineCountOld()[currentItem], difference->GetLineCountNew()[currentItem]);
                    }
                }
            }
        )
    }

    void GitService::GetDifferenceIndexFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine)
    {
        TRY_CATCH(
            assert(!IsBlank(filePath));
            std::wstring lineStr = noOfLine > -1 ? (L"--unified=" + to_wstring(noOfLine) + L" ") : L"";
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff --cached " + lineStr + Concat(hashIDs, L" ")), diff);
        )
    }

    void GitService::GetDifferenceWorkingFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine)
    {
        TRY_CATCH(
            assert(!IsBlank(filePath));
            std::wstring lineStr = noOfLine > -1 ? (L"--unified=" + to_wstring(noOfLine) + L" ") : L"";
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff " + lineStr + Concat(hashIDs, L" ") + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        )
    }

    void GitService::GetDifferenceFile(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine)
    {
        TRY_CATCH(
            assert(!IsBlank(filePath));
            std::wstring lineStr = noOfLine > -1 ? (L"--unified=" + to_wstring(noOfLine) + L" ") : L"";
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff HEAD " + lineStr + Concat(hashIDs, L" ") + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        )
    }

    void GitService::GetDifferenceCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, std::shared_ptr<GitDifference> diff, int64_t noOfLine)
    {
        TRY_CATCH(
            assert(!IsBlank(filePath));
            std::wstring lineStr = noOfLine > -1 ? (L"--unified=" + to_wstring(noOfLine) + L" ") : L"";
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff " + lineStr + fromHashID + L"..." + toHashID + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        )
    }

    std::wstring GitService::GetGitLogSearchCriteriaString(const GitLogSearchCriteria *searchCriteria)
    {
        std::wstring optionStr = L"";
        TRY_CATCH(
            if (searchCriteria == nullptr)
                return optionStr;
                
            // Commit Limiting
            if (searchCriteria->GetLogCount() > 0)
                optionStr += L" -" + std::to_wstring(searchCriteria->GetLogCount());
            
            if (searchCriteria->GetSkip() > 0)
                optionStr += L" --skip=" + std::to_wstring(searchCriteria->GetSkip());

            if (!IsBlank(searchCriteria->GetDateAfter()))
                optionStr += L" --after=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetDateAfter()) + L"\"";

            if (!IsBlank(searchCriteria->GetDateBefore()))
                optionStr += L" --before=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetDateBefore()) + L"\"";
            
            if (!IsBlank(searchCriteria->GetAuthor()))
                optionStr += L" --author=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetAuthor()) + L"\"";

            if (!IsBlank(searchCriteria->GetCommitter()))
                optionStr += L" --committer=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetCommitter()) + L"\"";

            if (!IsBlank(searchCriteria->GetGrep()))
                optionStr += L" --grep=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetGrep()) + L"\"";
            
            if (searchCriteria->GetIsGrepAllMatch())
                optionStr += L" --all-match";
            
            if (searchCriteria->GetIsGrepInvertGrep())
                optionStr += L" --invert-grep";
            
            if (searchCriteria->GetIsPatternRegexpIgnoreCase())
                optionStr += L" --regexp-ignore-case";

            if (searchCriteria->GetIsPatternExtendedRegexp())
                optionStr += L" --extended-regexp";

            if (searchCriteria->GetIsPatternFixedStrings())
                optionStr += L" --fixed-strings";

            if (searchCriteria->GetIsMerges())
                optionStr += L" --merges";

            if (searchCriteria->GetIsNoMerges())
                optionStr += L" --no-merges";

            if (searchCriteria->GetMinParents() >= 0)
                optionStr += L" --min-parents=" + std::to_wstring(searchCriteria->GetMinParents());
            
            if (searchCriteria->GetMaxParents() >= 0)
                optionStr += L" --max-parents=" + std::to_wstring(searchCriteria->GetMaxParents());
            
            if (searchCriteria->GetIsFirstParent())
                optionStr += L" --first-parent";

            if (searchCriteria->GetIsAll())
                optionStr += L" --all";

            if (searchCriteria->GetIsAllBranches())
                optionStr += L" --branches";
            else if (!IsBlank(searchCriteria->GetBranches()))
                optionStr += L" --branches=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetBranches()) + L"\"";
            
            if (searchCriteria->GetIsAllTags())
                optionStr += L" --tags";
            else if (!IsBlank(searchCriteria->GetTags()))
                optionStr += L" --tags=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetTags()) + L"\"";
            
            if (searchCriteria->GetIsAllRemotes())
                optionStr += L" --remotes";
            else if (!IsBlank(searchCriteria->GetRemotes()))
                optionStr += L" --remotes=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetRemotes()) + L"\"";
            
            if (searchCriteria->GetIsAllGlob())
                optionStr += L" --glob";
            else if (!IsBlank(searchCriteria->GetGlob()))
                optionStr += L" --glob=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetGlob()) + L"\"";
            
            if (!IsBlank(searchCriteria->GetExcludeGlob()))
                optionStr += L" --exclude=\"" + GetEscapeString(EscapeStringType::DoubleQuote, searchCriteria->GetExcludeGlob()) + L"\"";
            
            // History Simplification
            if (searchCriteria->GetIsSimplifyByDecoration())
                optionStr += L" --simplify-by-decoration";

            if (searchCriteria->GetIsShowPulls())
                optionStr += L" --show-pulls";

            if (searchCriteria->GetIsFullHistory())
                optionStr += L" --full-history";
            
            if (searchCriteria->GetIsSparse())
                optionStr += L" --sparse";

            if (searchCriteria->GetIsSimplifyMerges())
                optionStr += L" --simplify-merges";

            if (!IsBlank(searchCriteria->GetAncestryPath()))
                optionStr += L" --ancestry-path=" + searchCriteria->GetAncestryPath();
            
            switch (searchCriteria->GetOrderBy())
            {
            case GitLogOrderBy::NA:
                break;
            case GitLogOrderBy::Date:
                optionStr += L" --date-order";
                break;
            case GitLogOrderBy::AuthorDate:
                optionStr += L" --author-date-order";
                break;
            case GitLogOrderBy::Topo:
                optionStr += L" --topo-order";
                break;
            case GitLogOrderBy::Reverse:
                optionStr += L" --reverse";
                break;            
            default:
                assert(false);
                break;
            }

            // Revision Range
            if (!IsBlank(searchCriteria->GetRevisionFrom()) || !IsBlank(searchCriteria->GetRevisionTo())) {
                std::wstring revisionStr = L"";
                if (!IsBlank(searchCriteria->GetRevisionFrom()))
                    revisionStr += searchCriteria->GetRevisionFrom();
                revisionStr += L"..";
                if (!IsBlank(searchCriteria->GetRevisionTo()))
                    revisionStr += searchCriteria->GetRevisionTo();
                optionStr += L" " + revisionStr;
            }

            // path must be at the end            
            if (!searchCriteria->GetPaths().empty()) {
                optionStr += L" --";
                for (const std::wstring &path : searchCriteria->GetPaths()) {
                    optionStr += L" " + path;
                }
            }
        )
        return optionStr;
    }

    void GitService::ParseGitLog(const std::wstring &str, std::vector<std::shared_ptr<GitLog>> &logs)
    {

    }
    
    void GitService::GetLogs(const LogProperty *logProperty, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria, std::vector<std::shared_ptr<GitLog>> &logs)
    {
        //TRY_CATCH(
            std::wstring optionStr = GetGitLogSearchCriteriaString(searchCriteria);

            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git log --decorate=full " + optionStr);
        //)
    }

    void GitService::GetLogDetail(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitLog> log)
    {

    }

    void GitService::Stage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git add \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        )
    }
    void GitService::StageAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git add *");
        )
    }

    void GitService::Unstage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git reset \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        )
    }

    void GitService::UnstageAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git reset");
        )
    }

    void GitService::Commit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &command)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git commit -m \"" + GetEscapeString(EscapeStringType::DoubleQuote, command) + L"\"");
        )
    }

    void GitService::DiscardFile(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        )
    }

    void GitService::RestoreFileToParticularCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath, const std::wstring &hashID)
    {
        TRY_CATCH(
            assert(!IsBlank(hashID));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git restore --source=" + hashID + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        )        
    }

    void GitService::ResetCommit(const LogProperty *logProperty, const std::wstring &workspace,  const GitResetMode &resetMode, const std::wstring &hashID)
    {
        TRY_CATCH(
            std::wstring modeStr = L"";
            switch (resetMode)
            {
            case GitResetMode::NA:
                break;
            case GitResetMode::Soft:
                modeStr = L"--soft";
                break;
            case GitResetMode::Mixed:
                modeStr = L"--mixed";
                break;
            case GitResetMode::Hard:
                modeStr = L"--hard";
                break;
            case GitResetMode::Merge:
                modeStr = L"--merge";
                break;
            case GitResetMode::Keep:
                modeStr = L"--keep";
                break;        
            default:
                assert(false);
                break;
            }
            Trim(modeStr);
            modeStr += L" ";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git reset " + modeStr + hashID);
        )
    }

    void GitService::ReverseCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &hashID)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git revert " + hashID);
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
            if (!IsBlank(cmdResult)) {
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
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
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
            if (!IsBlank(cmdResult)) {
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
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
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
}