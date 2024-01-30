#include "git_service.hpp"

#include <assert.h>
#include <filesystem>
#include <map>
#include <math.h>
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

    const std::wstring hashIDPrefix = L"commit ";
    const std::wstring authorPrefix = L"Author:";
    const std::wstring authorDatePrefix = L"AuthorDate:";
    const std::wstring commitPrefix = L"Commit:";
    const std::wstring commitDatePrefix = L"CommitDate:";
    const std::wstring headerPrefix  = L"HEAD -> ";

    const std::wstring remoteMirrorFetch = L"(fetch)";
    const std::wstring remoteMirrorPush = L"(push)";

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
                    std::vector<std::wstring> tokens = SplitString(line.substr(remoteBranchPrefix.length()), { L"..." });
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

    void GitService::Clone(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &url, const std::wstring &branch, const int64_t &depth)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, 
                L"git clone " + url + (!branch.empty() ? (L" -b " + branch): L"") + (depth > 0 ? (L" --depth " + std::to_wstring(depth)) : L""));
        )
    }

    void GitService::CheckOut(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branch)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + branch);
        )
    }

    void GitService::GetRemote(const LogProperty *logProperty, const std::wstring &workspace, std::vector<std::shared_ptr<GitRemote>> &remotes)
    {
        TRY_CATCH(
            std::wstring result = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git remote -v");
            if (IsBlank(result))
                return;
            std::vector<std::wstring> lines = SplitStringByLine(result);
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, { L" ", L"\t" });
                if (tokens.size() != (size_t)3)
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected Remote: " + line);
                DECLARE_SPTR(GitRemote, remote);
                remote->SetName(tokens[0]);
                remote->SetURL(tokens[1]);
                if (tokens[2] == remoteMirrorFetch)
                    remote->SetMirror(GitRemoteMirror::Fetch);
                else if (tokens[2] == remoteMirrorPush)
                    remote->SetMirror(GitRemoteMirror::Push);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected Remote Mirror: " + line);
                remotes.push_back(remote);
            }
        )
    }

    void GitService::AddRemote(const LogProperty *logProperty, const std::wstring &workspace, const GitRemoteAddOption *option)
    {
        TRY_CATCH(
            assert(option != nullptr);
            std::wstring optionStr = L"";
            switch (option->GetMirror())
            {
            case GitRemoteMirror::NA:
                break;
            case GitRemoteMirror::Fetch:
                optionStr += L" --mirror=fetch";
                break;
            case GitRemoteMirror::Push:
                optionStr += L" --mirror=push";
                break;            
            default:
                assert(false);
                break;
            }

            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git remote " + optionStr + L" " + option->GetName() + L" " + option->GetURL());
        )
    }

    void GitService::RenameRemote(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &oldName, const std::wstring &newName)
    {
        TRY_CATCH(
            assert(!IsBlank(oldName));
            assert(!IsBlank(newName));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git rename " + oldName + L" " + newName);
        )
    }
    
    void GitService::RemoveRemote(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &name)
    {
        TRY_CATCH(
            assert(!IsBlank(name));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git remove " + name);
        )
    }

    void GitService::FetchAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git fetch --all");
        )
    }

    void GitService::Pull(const LogProperty *logProperty, const std::wstring &workspace, const GitPullOption *option)
    {
        TRY_CATCH(
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->GetIsQuite())
                    optionStr += L" --quiet";

                switch (option->GetRecurseSubmodules())
                {
                case GitPullOptionRecurseSubmodulesMode::Default:
                    break;
                case GitPullOptionRecurseSubmodulesMode::Yes:
                    optionStr += L" --recurse-submodules=yes";
                    break;
                case GitPullOptionRecurseSubmodulesMode::OnDemand:
                    optionStr += L" --recurse-submodules=on-demand";
                    break;
                case GitPullOptionRecurseSubmodulesMode::No:
                    optionStr += L" --recurse-submodules=no";
                    break;                
                default:
                    assert(false);
                    break;
                }
                for (const std::wstring &str : option->GetRepositories()) {
                    optionStr += L" " + str;
                }
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git pull " + optionStr);
        )
    }

    void GitService::Push(const LogProperty *logProperty, const std::wstring &workspace, const GitPushOption *option)
    {
        TRY_CATCH(
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->GetIsForce())
                    optionStr += L" --force";

                if (option->GetIsQuite())
                    optionStr += L" --quiet";

                for (const std::wstring &str : option->GetRepositories()) {
                    optionStr += L" " + str;
                }
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git push " + optionStr);
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

    void GitService::ParseGitLogGraph(const std::wstring &str, std::vector<std::shared_ptr<GitLog>> &logs)
    {
        TRY_CATCH(
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                size_t pos = line.find(L"*");
                if (pos == std::wstring::npos)
                    continue;
                DECLARE_SPTR(GitLog, log);
                if (pos > 0) {
                    log->SetColumnIndex((size_t)std::floor(pos/2));
                } else
                    log->SetColumnIndex(0);
                std::wregex pattern(L"\\(([^)]+)\\)");
                std::wsmatch match;
                std::wstring::const_iterator searchStart(line.cbegin());
                size_t cnt = 0;
                while (std::regex_search(searchStart, line.cend(), match, pattern)) {
                    switch (cnt)
                    {
                    case 0:
                        log->SetHashID(match[1]);
                        break;
                    case 1:
                        log->SetAbbreviatedHashID(match[1]);
                        break;
                    case 2:
                        log->SetTreeHashID(match[1]);
                        break;
                    case 3:
                        log->SetAbbreviatedTreeHashID(match[1]);
                        break;
                    case 4: {
                        std::wstring parentID = match[1];
                        std::vector<std::wstring> tokens = SplitString(parentID, { L" " });
                        for (std::wstring token : tokens) {
                            if (IsBlank(token))
                                continue;
                            Trim(token);
                            log->InsertParentHashIDs(token);
                        }
                        break;
                    }
                    case 5: {
                        std::wstring parentID = match[1];
                        std::vector<std::wstring> tokens = SplitString(parentID, { L" " });
                        for (std::wstring token : tokens) {
                            if (IsBlank(token))
                                continue;
                            Trim(token);
                            log->InsertAbbreviatedParentHashIDs(token);
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    cnt++;
                    searchStart = match.suffix().first;
                }
                logs.push_back(log);
            }
        )
    }

    std::time_t GitService::ParseGitLogDatetime(const std::wstring &datimeStr)
    {
        std::time_t time = -1;
        TRY_CATCH(
            struct tm tm;
            strptime(wstr2str(datimeStr).c_str(), "%a %b %d %H:%M:%S %Y %z", &tm);
            time = mktime(&tm);
        )
        return time;
    }

    void GitService::ParseGitLog(const std::wstring &str, std::shared_ptr<GitLog> log)
    {
        TRY_CATCH(
            if (!HasPrefix(str, L"commit "))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"str not has prefix \"commit \"");

            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                if (IsBlank(line)) {
                    // first line is title
                    // second line is full message
                    // handle in seccod loop
                    break;
                } else if (HasPrefix(line, hashIDPrefix)) {
                    std::wstring tmpLine = line.substr(hashIDPrefix.length());
                    Trim(tmpLine);
                    std::vector<std::wstring> tokens = SplitString(tmpLine, { L" " });
                    if (tokens.empty())
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"HashID missing: " + str);
                    if (!IsBlank(log->GetHashID()) && log->GetHashID() != tokens.at(0))
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"HashID " + log->GetHashID() + L" + not match: " + str);
                    log->SetHashID(tokens.at(0));
                    if (tmpLine.find(L"(") != std::wstring::npos) {
                        std::wregex pattern(L"\\((.*?)\\)");
                        std::wsmatch matches;
                        if (std::regex_search(tmpLine, matches, pattern)) {
                            std::wstring tagsStr = matches[1].str();
                            Trim(tagsStr);
                            if (HasPrefix(tagsStr, headerPrefix)) {
                                log->SetIsHead(true);
                                tagsStr = tagsStr.substr(headerPrefix.length());
                                Trim(tagsStr);
                            } else
                                log->SetIsHead(false);
                            std::vector<std::wstring> tagTokens = SplitString(tagsStr, { L"," });
                            for (std::wstring tag : tagTokens) {
                                Trim(tag);
                                log->InsertTags(tag);
                            }
                        }
                    }
                } else if (HasPrefix(line, authorPrefix)) {
                    std::wstring tmpLine = line.substr(authorPrefix.length());
                    Trim(tmpLine);
                    // name
                    std::wstring name = tmpLine.substr(0, tmpLine.find(L"<") - 1);
                    Trim(name);
                    log->SetAuthor(name);
                    //email
                    std::wregex pattern(L"<(.+?)>");
                    std::wsmatch matches;
                    if (std::regex_search(tmpLine, matches, pattern)) {
                        std::wstring email = matches[1].str();
                        Trim(email);
                        log->SetAuthorEmail(email);
                    }
                } else if (HasPrefix(line, authorDatePrefix)) {
                    std::wstring tmpLine = line.substr(authorDatePrefix.length());
                    Trim(tmpLine);
                    log->SetAuthorDate(GitService::ParseGitLogDatetime(tmpLine));
                    log->SetAuthorDateStr(tmpLine);
                } else if (HasPrefix(line, commitPrefix)) {
                    std::wstring tmpLine = line.substr(commitPrefix.length());
                    Trim(tmpLine);
                    // name
                    std::wstring name = tmpLine.substr(0, tmpLine.find(L"<") - 1);
                    Trim(name);
                    log->SetCommitter(name);
                    //email
                    std::wregex pattern(L"<(.+?)>");
                    std::wsmatch matches;
                    if (std::regex_search(tmpLine, matches, pattern)) {
                        std::wstring email = matches[1].str();
                        Trim(email);
                        log->SetCommitterEmail(email);
                    }
                } else if (HasPrefix(line, commitDatePrefix)) {
                    std::wstring tmpLine = line.substr(commitDatePrefix.length());
                    Trim(tmpLine);
                    log->SetCommitDate(GitService::ParseGitLogDatetime(tmpLine));
                    log->SetCommitDateStr(tmpLine);
                }
            }

            // Title and Message
            bool isTitle = false;
            bool isMessage = false;
            std::wstring title = L"";
            std::wstring message = L"";
            std::wstring messagePrefix = L"    ";
            for (const std::wstring &line : lines) {
                if (isTitle) {
                    if (!line.empty()) {
                        std::wstring tmpStr = line;
                        if (tmpStr.length() >= messagePrefix.length())
                            tmpStr = tmpStr.substr(messagePrefix.length());
                        if (!title.empty())
                            title += L"\n";
                        title += tmpStr;
                    } else {
                        isTitle = false;
                        isMessage = true;
                    }
                } else if (isMessage) {
                    std::wstring tmpStr = line;
                        if (tmpStr.length() >= messagePrefix.length())
                            tmpStr = tmpStr.substr(messagePrefix.length());
                    if (!message.empty())
                        message += L"\n";
                    message += tmpStr;
                } else {
                    if (line.empty()) {
                        if (title.empty())
                            isTitle = true;
                    }
                }
            }
            log->SetTitle(title);
            log->SetMessage(message);
            log->SetFullMessage(str);
        )
    }
    
    void GitService::GetLogs(const LogProperty *logProperty, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria, std::vector<std::shared_ptr<GitLog>> &logs)
    {
        TRY_CATCH(
            ParseGitLogGraph(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git log --graph --oneline --pretty=format:\"(%H)(%h)(%T)(%t)(%P)(%p)\" " + GetGitLogSearchCriteriaString(searchCriteria)),
                logs);
            std::map<std::wstring, std::shared_ptr<GitLog>> logMap;
            for (auto log : logs) {
                logMap.insert({log->GetHashID(), log});
            }
            std::wstring logDetail = L"";
            std::wstring currentHashID = L"";
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git log --pretty=fuller" + GetGitLogSearchCriteriaString(searchCriteria)));
            for (const std::wstring &line : lines) {
                if (HasPrefix(line, hashIDPrefix)) {
                    if (!logDetail.empty()) {
                        ParseGitLog(logDetail, logMap.at(currentHashID));
                    }
                    logDetail = line;
                    currentHashID = line.substr(hashIDPrefix.size());
                    Trim(currentHashID);
                    currentHashID = SplitString(currentHashID, { L" " }).at(0);
                    Trim(currentHashID);
                } else {
                    if (!logDetail.empty())
                        logDetail += L"\n";
                    logDetail += line;
                }
            }
            if (!logDetail.empty()) {
                ParseGitLog(logDetail, logMap.at(currentHashID));
            }
        )
    }

    void GitService::GetLog(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &hashID, std::shared_ptr<GitLog> log)
    {
        TRY_CATCH(
            assert(!IsBlank(hashID));
            GitService::ParseGitLog(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git log --pretty=fuller -n 1 " + hashID), log);
        )
    }

    void GitService::GetDifferenceSummary(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, std::shared_ptr<GitDifferenceSummary> summary)
    {
        TRY_CATCH(
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff " + Concat(hashIDs, L" ") + L" --numstat"));
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, { L"\t" }, { L"\"" }, { L"\"" }, { L"\\" });
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
                        std::vector<std::wstring> tokens = SplitString(tmpStr, { L" " });
                        if (tokens.size() != 2)
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        // old
                        std::vector<std::wstring> lineCountOld = SplitString(tokens[0], { L"," });
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
                        std::vector<std::wstring> lineCountNew = SplitString(tokens[1], { L"," });
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

    void GitService::Amend(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY_CATCH(
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git commit --amend");
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