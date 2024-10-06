#include "git_service.hpp"

#include <assert.h>
#include <filesystem>
#include <map>
#include <math.h>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#ifdef _WIN32
#include <ctime>
#endif

#include "config.hpp"
#include "config_builder.hpp"
#include "time_helper.hpp"
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
    const std::wstring tagPrefix = L"tag:";

    const std::wstring remoteMirrorFetch = L"(fetch)";
    const std::wstring remoteMirrorPush = L"(push)";

    GitFileStatus GitService::ParseGitFileStatus(const wchar_t &c)
    {
        TRY
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
        CATCH
        return GitFileStatus::NA;
    }
    
    std::wstring GitService::Execute(const LogProperty *logProperty, const std::wstring &command)
    {
        TRY
            return ProcessService::Execute(logProperty, GIT_LOG_ID, command);
        CATCH
        return L"";
    }
    
    std::wstring GitService::GetVersion(const LogProperty *logProperty)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        CATCH
        return L"";
    }

    bool GitService::IsGitResponse(const LogProperty *logProperty, const std::wstring &workspace)
    {
        bool result = false;
        try {
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace,
                L"git rev-parse --is-inside-work-tree");
            result = Find(cmdResult, L"true") != std::wstring::npos;
        } catch (...) {
            result = false;
        }
        return result;
    }

    void GitService::GetStatus(const LogProperty *logProperty, const std::wstring &workspace, const GitStatusSearchCriteria *searchCriteria, std::shared_ptr<GitStatus> status)
    {
        TRY
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->GetIsWithIgnoreFiles())
                    optionStr += L" --ignored";
            }

            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git status -b -s" + optionStr));
            
            std::wstring localBrachPrefix = L"## No commits yet on ";
            std::wstring remoteBranchPrefix = L"## ";
            size_t prefixLength = 2;
            for (auto line : lines) {
                if (line.length() <= 2)
                    continue;

                // for branch
                if (IsStartWith(line, localBrachPrefix)) {
                    // Note: For non-remote git response, no commit return is L"## No commits yet on main"
                    // Note: For remote git response, no commit return is L"## main...origin/main"
                    std::wstring subLine = line.substr(localBrachPrefix.length());
                    Trim(subLine);
                    status->SetBranch(subLine);
                    status->SetRemoteBranch(L"");
                    continue;
                } else if (IsStartWith(line, remoteBranchPrefix)) {
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
        CATCH
    }

    // Initialization
    void GitService::Initialize(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git init");
        CATCH
    }

    void GitService::Clone(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &url, const GitCloneOption *option)
    {
        TRY
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (!IsBlank(option->GetBranch()))
                    optionStr += L" -b " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, option->GetBranch());
                if (option->GetDepth() > 0)
                    optionStr +=L" --depth " + std::to_wstring(option->GetDepth());
                if (option->GetIsQuiet())
                    optionStr +=L" --quiet";
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git clone " + url + optionStr);
        CATCH
    }

    void GitService::GetRemote(const LogProperty *logProperty, const std::wstring &workspace, std::vector<std::shared_ptr<GitRemote>> &remotes)
    {
        TRY
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
        CATCH
    }

    void GitService::AddRemote(const LogProperty *logProperty, const std::wstring &workspace, const GitRemoteAddOption *option)
    {
        TRY
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
        CATCH
    }

    void GitService::RenameRemote(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &oldName, const std::wstring &newName)
    {
        TRY
            assert(!IsBlank(oldName));
            assert(!IsBlank(newName));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git rename " + oldName + L" " + newName);
        CATCH
    }
    
    void GitService::RemoveRemote(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &name)
    {
        TRY
            assert(!IsBlank(name));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git remove " + name);
        CATCH
    }

    void GitService::FetchAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git fetch --all");
        CATCH
    }

    void GitService::Pull(const LogProperty *logProperty, const std::wstring &workspace, const GitPullOption *option)
    {
        TRY
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
        CATCH
    }

    void GitService::Push(const LogProperty *logProperty, const std::wstring &workspace, const GitPushOption *option)
    {
        TRY
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
        CATCH
    }

    std::wstring GitService::GetGitLogSearchCriteriaString(const GitLogSearchCriteria *searchCriteria)
    {
        std::wstring optionStr = L"";
        TRY
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
        CATCH
        return optionStr;
    }

    void GitService::ParseGitLogGraph(const std::wstring &str, std::vector<std::shared_ptr<GitLog>> &logs)
    {
        TRY
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                size_t pos = Find(line, L"*");
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
        CATCH
    }

    std::time_t GitService::ParseGitLogDatetime(const std::wstring &datimeStr)
    {
        std::time_t time = -1;
        TRY
            time = ParseDatetime(datimeStr, L"%a %b %d %H:%M:%S %Y %z");
        CATCH
        return time;
    }

    void GitService::ParseGitLog(const std::wstring &str, std::shared_ptr<GitLog> log)
    {
        TRY
            if (!IsStartWith(str, L"commit "))
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"str not has prefix \"commit \"");

            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                if (IsBlank(line)) {
                    // first line is title
                    // second line is full message
                    // handle in seccod loop
                    break;
                } else if (IsStartWith(line, hashIDPrefix)) {
                    std::wstring tmpLine = line.substr(hashIDPrefix.length());
                    Trim(tmpLine);
                    std::vector<std::wstring> tokens = SplitString(tmpLine, { L" " });
                    if (tokens.empty())
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"HashID missing: " + str);
                    if (!IsBlank(log->GetHashID()) && log->GetHashID() != tokens.at(0))
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"HashID " + log->GetHashID() + L" + not match: " + str);
                    log->SetHashID(tokens.at(0));
                    if (Find(tmpLine, L"(") != std::wstring::npos) {
                        std::wregex pattern(L"\\((.*?)\\)");
                        std::wsmatch matches;
                        if (std::regex_search(tmpLine, matches, pattern)) {
                            std::wstring tagsStr = matches[1].str();
                            Trim(tagsStr);
                            if (IsStartWith(tagsStr, headerPrefix)) {
                                log->SetIsHead(true);
                                tagsStr = tagsStr.substr(headerPrefix.length());
                                Trim(tagsStr);
                            } else
                                log->SetIsHead(false);

                            std::vector<std::wstring> tokens = SplitString(tagsStr, { L"," });
                            for (std::wstring token : tokens) {
                                Trim(token);
                                if (IsStartWith(token, tagPrefix)) {
                                    token = token.substr(tagPrefix.length());
                                    Trim(token);
                                    log->InsertTags(token);
                                } else {
                                    log->InsertBranches(token);
                                }
                            }
                        }
                    }
                } else if (IsStartWith(line, authorPrefix)) {
                    std::wstring tmpLine = line.substr(authorPrefix.length());
                    Trim(tmpLine);
                    // name
                    std::wstring name = tmpLine.substr(0, Find(tmpLine, L"<") - 1);
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
                } else if (IsStartWith(line, authorDatePrefix)) {
                    std::wstring tmpLine = line.substr(authorDatePrefix.length());
                    Trim(tmpLine);
                    log->SetAuthorDate(GitService::ParseGitLogDatetime(tmpLine));
                    log->SetAuthorDateStr(tmpLine);
                } else if (IsStartWith(line, commitPrefix)) {
                    std::wstring tmpLine = line.substr(commitPrefix.length());
                    Trim(tmpLine);
                    // name
                    std::wstring name = tmpLine.substr(0, Find(tmpLine, L"<") - 1);
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
                } else if (IsStartWith(line, commitDatePrefix)) {
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
        CATCH
    }
    
    void GitService::GetLogs(const LogProperty *logProperty, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria, std::vector<std::shared_ptr<GitLog>> &logs)
    {
        TRY
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
                if (IsStartWith(line, hashIDPrefix)) {
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
        CATCH
    }

    void GitService::GetCurrentLog(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitLog> log)
    {
        TRY
            DECLARE_SPTR(GitLogSearchCriteria, searchCriteria);
            searchCriteria->SetLogCount(1);
            std::vector<std::shared_ptr<GitLog>> logs;
            GitService::GetLogs(logProperty, workspace, searchCriteria.get(), logs);
            if (!logs.empty()) {
                log = std::dynamic_pointer_cast<GitLog>(logs.at(0)->Clone());
            }
        CATCH
    }

    void GitService::GetLog(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &hashID, std::shared_ptr<GitLog> log)
    {
        TRY
            assert(!IsBlank(hashID));
            GitService::ParseGitLog(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git log --pretty=fuller -n 1 " + hashID), log);
        CATCH
    }

    void GitService::GetTags(const LogProperty *logProperty, const std::wstring &workspace, const GitTagSearchCriteria *searchCriteria, std::vector<std::wstring> &tags)
    {
        TRY
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (!IsBlank(searchCriteria->GetContains()))
                    optionStr += L" --contains " + searchCriteria->GetContains();

                if (!IsBlank(searchCriteria->GetNoContains()))
                    optionStr += L" --no-contains " + searchCriteria->GetNoContains();

                if (!IsBlank(searchCriteria->GetOrderBy()))
                    optionStr += L" --sort=" + searchCriteria->GetOrderBy();
            }
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git tag -l"));
            tags.insert(tags.end(), lines.begin(), lines.end());
        CATCH
    }

    // void GitService::GetTag(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &tagName, std::shared_ptr<GitLog> log)
    // {
    //     TRY
    //         assert(!IsBlank(tagName));
    //          std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git show --format=fuller " + tagName));
    //         bool isAfterFirstHeader = false;
    //         std::wstring str = L"";
    //         for (const std::wstring &line : lines) {
    //             if (isAfterFirstHeader) {
    //                 if (line.length() > 0 && std::iswalnum(line.at(0)))
    //                     break;
    //                 str += line + L"\n";
    //             } else {
    //                 str += line + L"\n";
    //                 if (IsBlank(line))
    //                     isAfterFirstHeader = true;
    //             }
    //         }
    //         GitService::ParseGitLog(str, log);
    //     CATCH
    // }

    GitTagCurrentTag GitService::GetCurrentTag(const LogProperty *logProperty, const std::wstring &workspace)
    {
        GitTagCurrentTag currentTag;
        TRY
            std::wstring tagStr = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git describe --tags"))[0];
            std::vector<std::wstring> tokens = SplitString(tagStr, { L"-" });
            if (tokens.size() >= 3) {
                try
                {
                    currentTag.SetNoOfCommit(std::stoi(tokens[tokens.size() - 2]));
                    currentTag.SetHashID(tokens[tokens.size() - 1]);
                    tokens.pop_back();
                    tokens.pop_back();
                    currentTag.SetTagName(Concat(tokens, L"-"));
                }
                catch(...)
                {
                    currentTag.SetTagName(tagStr);
                }
            } else {
                currentTag.SetTagName(tagStr);
            }
        CATCH
        return currentTag;
    }

    void GitService::CreateTag(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &tagName, const GitTagCreateTagOption *option)
    {
        TRY
            assert(!IsBlank(tagName));
            std::wstring optionStr = L"";
            std::wstring suffixOptionStr = L"";
            if (option != nullptr) {
                switch (option->GetSign())
                {
                case GitTagCreateTagOptionSignMode::Default:
                    optionStr += L" -a";
                    break;
                case GitTagCreateTagOptionSignMode::NoSign:
                    optionStr += L" --no-sign";
                    break;
                case GitTagCreateTagOptionSignMode::Sign:
                    optionStr += L" -s";
                    break;
                case GitTagCreateTagOptionSignMode::LocalUser:
                    assert(!IsBlank(option->GetSignLocalUserKeyID()));
                    optionStr += L" -local-user=" + option->GetSignLocalUserKeyID();
                    break;
                default:
                    assert(false);
                    break;
                }

                if (option->GetIsForce())
                    optionStr += L" -f";
                
                if (!IsBlank(option->GetMessage()))
                    optionStr += L" -m \"" + GetEscapeString(EscapeStringType::DoubleQuote, option->GetMessage()) + L"\"";

                if (!IsBlank(option->GetHashID()))
                    suffixOptionStr += L" " + option->GetHashID();
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git tag" + optionStr + L" " + tagName + L" " + suffixOptionStr);
        CATCH
    }

    void GitService::Switch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &tagName, bool isForce)
    {
        TRY
            assert(!IsBlank(tagName));
            std::wstring optionStr = isForce ? L" -f" : L"";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout " + optionStr + L" " + tagName);
        CATCH
    }

    void GitService::SwitchReverse(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git switch -");
        CATCH
    }

    void GitService::DeleteTag(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &tagName)
    {
        TRY
            assert(!IsBlank(tagName));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git tag -d " + tagName);
        CATCH
    }

    void GitService::ParseGitBranch(const std::wstring &str, std::shared_ptr<GitBranch> branch)
    {
        TRY
            std::wstring tmpStr = str;
            std::wstring checkoutPrefix = L"*";
            if (IsStartWith(tmpStr, checkoutPrefix)) {
                branch->SetIsActive(true);
                tmpStr = tmpStr.substr(checkoutPrefix.size());
            }
            Trim(tmpStr);
            std::vector<std::wstring> tokens = SplitString(tmpStr, { L" ", L"\t"});
            if (tokens.size() < 3)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected git branch pattern: " + str);
            branch->SetName(tokens[0]);
            std::wstring hashID = tokens[1];
            bool isPointTo = hashID == L"->";
            std::wstring title = str.substr(Find(str, hashID) + hashID.size());
            Trim(title);
            if (isPointTo) {
                branch->SetPointToBranch(title);
            } else {
                branch->SetHashID(hashID);
                branch->SetTitle(title);
            }
        CATCH
    }

    // void GitService::GetCurrentBranch(const LogProperty *logProperty, const std::wstring &workspace, std::shared_ptr<GitBranch> branch)
    // {
    //     TRY
    //         std::wstring str = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch -v");
    //         if (str.empty())
    //             return;
    //         std::vector<std::wstring> lines = SplitStringByLine(str);
    //         if (!lines.empty()) {
    //             ParseGitBranch(lines[0], branch);
    //         }
    //     }
    //}

    std::wstring GitService::GetCurrentBranchName(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            return SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch --show-current"))[0];
        CATCH
        return L"";
    }

    void GitService::GetBranches(const LogProperty *logProperty, const std::wstring &workspace, std::vector<std::shared_ptr<GitBranch>> &branches)
    {
        TRY
            std::wstring str = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch -v --all");
            if (str.empty())
                return;
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                if (SplitString(str, { L" ", L"\t" }).size() > 2) {
                    DECLARE_SPTR(GitBranch, branch);
                    ParseGitBranch(line, branch);
                    branches.push_back(branch);
                }
            }
        CATCH
    }

    void GitService::CreateBranch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branchName, const GitBranchCreateBranchOption *option)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->GetIsForce())
                    optionStr += L" -f";

                switch (option->GetTrack())
                {
                case GitBranchCreateBranchOptionTrackMode::Default:
                    break;
                case GitBranchCreateBranchOptionTrackMode::No:
                    optionStr += L" --no-track";
                    break;
                case GitBranchCreateBranchOptionTrackMode::Direct:
                    optionStr += L" --track=direct";
                    break;
                case GitBranchCreateBranchOptionTrackMode::Inherit:
                    optionStr += L" --track=inherit";
                    break;
                default:
                    assert(false);
                    break;
                }

                if (option->GetIsRecurseSubmodules())
                    optionStr += L" --recurse-submodules";
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + branchName);
        CATCH
    }

    void GitService::SwitchBranch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branchName, const GitBranchSwitchBranchOption *option)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->GetIsQuite()) {
                    optionStr += L" -q";
                }
                if (option->GetIsDiscardChanges()) {
                    optionStr += L" --discard-changes";
                }
            }
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git switch" + optionStr + L" "+ branchName);
        CATCH
    }

    void GitService::RenameBranch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            assert(!IsBlank(oldBranchName));
            assert(!IsBlank(newBranchName));
            std::wstring optionStr = isForce ? L" -M" : L" -m";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + oldBranchName + L" " + newBranchName);
        CATCH
    }

    void GitService::CopyBranch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            assert(!IsBlank(oldBranchName));
            assert(!IsBlank(newBranchName));
            std::wstring optionStr = isForce ? L" -C" : L" -c";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + oldBranchName + L" " + newBranchName);
        CATCH
    }        
        
    void GitService::DeleteBranch(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &branchName, bool isForce)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = isForce ? L" -D" : L" -d";
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + branchName);
        CATCH
    }

    void GitService::GetDifferenceSummary(const LogProperty *logProperty, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs, std::shared_ptr<GitDifferenceSummary> summary)
    {
        TRY
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff " + Concat(hashIDs, L" ") + L" --numstat"));
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, { L"\t" }, { L"\"" }, { L"\"" }, { L"\\" });
                if (tokens.size() < 3)
                    continue;
                summary->InsertFiles(tokens.at(2));
                summary->InsertAddLineCounts((size_t)stoi(tokens.at(0)));
                summary->InsertDeleteLineCounts((size_t)stoi(tokens.at(1)));
            }
        CATCH
    }

    void GitService::ParseGitDiff(const std::wstring &str, std::shared_ptr<GitDifference> difference)
    {
        TRY
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
                    if (IsStartWith(line, filePathOldPrefix)) {
                        std::wstring tmpStr = line.substr(filePathOldPrefix.length());
                        Trim(tmpStr);
                        difference->SetFilePathOld(tmpStr);
                    } else if (IsStartWith(line, filePathNewPrefix)){
                        std::wstring tmpStr = line.substr(filePathNewPrefix.length());
                        Trim(tmpStr);
                        difference->SetFilePathNew(tmpStr);
                    } else if (IsStartWith(line, lineCountPrefix)) {
                        // sample new: @@ -1 +1,2 @@
                        // sample modify: @@ -116,10 +116,10 @@ xxxx
                        // 1. Chop to remove all @@
                        // 2. Get all numbers
                        std::wstring tmpStr = line.substr(lineCountPrefix.length());
                        tmpStr = tmpStr.substr(0, Find(tmpStr, lineCountPrefix));
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
                        if (!(IsStartWith(lineNumberStr, L"-") && lineNumberStr.length() > 1))
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
                        if (!(IsStartWith(lineNumberStr, L"+") && lineNumberStr.length() > 1))
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
        CATCH
    }

    void GitService::GetDifferenceIndexFile(const LogProperty *logProperty, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath,  std::shared_ptr<GitDifference> diff)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->GetNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->GetNoOfLines());
                
                if (!searchCriteria->GetHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->GetHashIDs(), L" ");
            }
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff --cached" + optionStr), diff);
        CATCH
    }

    void GitService::GetDifferenceWorkingFile(const LogProperty *logProperty, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath, std::shared_ptr<GitDifference> diff)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->GetNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->GetNoOfLines());
                
                if (!searchCriteria->GetHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->GetHashIDs(), L" ");
            }
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff" + optionStr + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        CATCH
    }

    void GitService::GetDifferenceFile(const LogProperty *logProperty, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &filePath, std::shared_ptr<GitDifference> diff)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->GetNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->GetNoOfLines());
                
                if (!searchCriteria->GetHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->GetHashIDs(), L" ");
            }
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff HEAD" + optionStr + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        CATCH
    }

    void GitService::GetDifferenceCommit(const LogProperty *logProperty, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria, const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, std::shared_ptr<GitDifference> diff)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->GetNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->GetNoOfLines());
            }
            ParseGitDiff(ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git diff" + optionStr + L" " + fromHashID + L"..." + toHashID + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""), diff);
        CATCH
    }

    void GitService::Stage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git add \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }
    void GitService::StageAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git add *");
        CATCH
    }

    void GitService::Unstage(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git reset \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::UnstageAll(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git reset");
        CATCH
    }

    void GitService::Commit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &command)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git commit -m \"" + GetEscapeString(EscapeStringType::DoubleQuote, command) + L"\"");
        CATCH
    }

    void GitService::Amend(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git commit --amend");
        CATCH  
    }

    void GitService::DiscardFile(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git checkout \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::RestoreFileToParticularCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &filePath, const std::wstring &hashID)
    {
        TRY
            assert(!IsBlank(hashID));
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git restore --source=" + hashID + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::ResetCommit(const LogProperty *logProperty, const std::wstring &workspace,  const GitResetMode &resetMode, const std::wstring &hashID)
    {
        TRY
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
        CATCH
    }

    void GitService::ReverseCommit(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &hashID)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git revert " + hashID);
        CATCH
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
        TRY
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
        CATCH
    }

    std::wstring GitService::GetConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        std::wstring result = L"";
        TRY
            try {
                result = GitService::GetLocalConfig(logProperty, workspace, key);           
            } catch (...) {
                result = L"";
            }
            if (result.empty())
                result = GitService::GetGlobalConfig(logProperty, key);
        CATCH
        return result;
    }

    std::wstring GitService::GetUserName(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            return GitService::GetConfig(logProperty, workspace, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";        
    }
    
    std::wstring GitService::GetUserEmail(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            return GitService::GetConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
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
        TRY
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global --list");
            if (!IsBlank(cmdResult)) {
                DECLARE_SPTR(Config, element);
                DECLARE_UPTR(ConfigBuilder, reader);
                reader->Deserialize(cmdResult, element);
                for (auto it : element->GetConfigs()) {
                    if (!element->IsValue(it.first))
                        continue;

                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->SetUserEmail(it.second);
                    }
                    config->InsertConfigs(it.first, it.second);
                }

            }
        CATCH
    }

    std::wstring GitService::GetGlobalConfig(const LogProperty *logProperty, const std::wstring &key)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key);
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        CATCH
        return L"";
    }
    
    void GitService::SetGlobalConfig(const LogProperty *logProperty, const std::wstring &key, const std::wstring &value)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, L"git config --global " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        CATCH
    }

    std::wstring GitService::GetGlobalUserName(const LogProperty *logProperty)
    {
        TRY
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";
    }

    void GitService::SetGlobalUserName(const LogProperty *logProperty, const std::wstring &value)
    {
        TRY
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_NAME, value);
        CATCH
    }

    std::wstring GitService::GetGlobalUserEmail(const LogProperty *logProperty)
    {
        TRY
            return GitService::GetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
    }

    void GitService::SetGlobalUserEmail(const LogProperty *logProperty, const std::wstring &value)
    {
        TRY
            GitService::SetGlobalConfig(logProperty, GIT_CONFIG_USER_EMAIL, value);
        CATCH
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
        TRY
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local --list");
            if (!IsBlank(cmdResult)) {
                DECLARE_SPTR(Config, element);
                DECLARE_UPTR(ConfigBuilder, reader);

                reader->Deserialize(cmdResult, element);
                for (auto it : element->GetConfigs()) {
                    if (!element->IsValue(it.first))
                        continue;
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->SetUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->SetUserEmail(it.second);
                    }
                    config->InsertConfigs(it.first, it.second);
                }
            }
        CATCH
    }

    std::wstring GitService::GetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key);
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        CATCH
        return L"";
    }

    void GitService::SetLocalConfig(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &key, const std::wstring &value)
    {
        TRY
            ProcessService::Execute(logProperty, GIT_LOG_ID, workspace, L"git config --local " + key + L" \"" + GetEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        CATCH
    }

    std::wstring GitService::GetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";
    }

    void GitService::SetLocalUserName(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_NAME, value);
        CATCH
    }

    std::wstring GitService::GetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace)
    {
        TRY
            return GitService::GetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
    }

    void GitService::SetLocalUserEmail(const LogProperty *logProperty, const std::wstring &workspace, const std::wstring &value)
    {
        TRY
            GitService::SetLocalConfig(logProperty, workspace, GIT_CONFIG_USER_EMAIL, value);
        CATCH
    }
}