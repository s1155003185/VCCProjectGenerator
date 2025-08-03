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
#include "log_config.hpp"
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

    std::wstring GitService::Execute(const LogConfig *logConfig, const std::wstring &command)
    {
        TRY
            return ProcessService::Execute(logConfig, GIT_LOG_ID, command);
        CATCH
        return L"";
    }
    
    std::wstring GitService::GetVersion(const LogConfig *logConfig)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, L"git --version");
            std::wsmatch m;
            if (std::regex_search(cmdResult, m, std::wregex(L"[0-9]+.[0-9]+.[0-9]+")))
                return m[0];
        CATCH
        return L"";
    }

    bool GitService::IsGitResponse(const LogConfig *logConfig, const std::wstring &workspace)
    {
        bool result = false;
        try {
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace,
                L"git rev-parse --is-inside-work-tree");
            result = Find(cmdResult, L"true") != std::wstring::npos;
        } catch (...) {
            result = false;
        }
        return result;
    }
    
    GitFileStatus ParseGitFileStatus(const wchar_t &c)
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
    
    std::shared_ptr<GitStatus> GitService::GetStatus(const LogConfig *logConfig, const std::wstring &workspace, const GitStatusSearchCriteria *searchCriteria)
    {
        auto status = std::make_shared<GitStatus>();
        TRY
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->getIsWithIgnoreFiles())
                    optionStr += L" --ignored";
            }

            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git status -b -s" + optionStr));
            
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
                    status->setBranch(subLine);
                    status->setRemoteBranch(L"");
                    continue;
                } else if (IsStartWith(line, remoteBranchPrefix)) {
                    std::vector<std::wstring> tokens = SplitString(line.substr(remoteBranchPrefix.length()), { L"..." });
                    status->setBranch(tokens.at(0));
                    if (tokens.size() > 1)
                        status->setRemoteBranch(tokens.at(1));
                    continue;
                }

                // for files
                GitFileStatus indexFileStatus = ParseGitFileStatus(line[0]);
                GitFileStatus workingTreeFileStatus = ParseGitFileStatus(line[1]);
                std::wstring filePath = line.size() > prefixLength ? line.substr(prefixLength) : L"";
                Trim(filePath);
                if (indexFileStatus == GitFileStatus::Ignored || indexFileStatus == GitFileStatus::Untracked) {
                    status->getWorkingTreeFiles()[indexFileStatus].push_back(filePath);
                    continue;
                }
                if (indexFileStatus != GitFileStatus::NA)
                    status->getIndexFiles()[indexFileStatus].push_back(filePath);
                if (workingTreeFileStatus != GitFileStatus::NA)
                    status->getWorkingTreeFiles()[workingTreeFileStatus].push_back(filePath);
            }
        CATCH
        return status;
    }

    // Initialization
    void GitService::InitializeGitResponse(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git init");
        CATCH
    }

    void GitService::CloneGitResponse(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &url, const GitCloneOption *option)
    {
        TRY
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (!IsBlank(option->getBranch()))
                    optionStr += L" -b " + getEscapeStringWithQuote(EscapeStringType::DoubleQuote, option->getBranch());
                if (option->getDepth() > 0)
                    optionStr +=L" --depth " + std::to_wstring(option->getDepth());
                if (option->getIsQuiet())
                    optionStr +=L" --quiet";
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git clone " + url + optionStr);
        CATCH
    }

    std::vector<std::shared_ptr<GitRemote>> GitService::GetRemote(const LogConfig *logConfig, const std::wstring &workspace)
    {
        std::vector<std::shared_ptr<GitRemote>> remotes;
        TRY
            std::wstring result = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git remote -v");
            if (IsBlank(result))
                return remotes;
            std::vector<std::wstring> lines = SplitStringByLine(result);
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, { L" ", L"\t" });
                if (tokens.size() != (size_t)3)
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected Remote: " + line);
                auto remote = std::make_shared<GitRemote>();
                remote->setName(tokens[0]);
                remote->setURL(tokens[1]);
                if (tokens[2] == remoteMirrorFetch)
                    remote->setMirror(GitRemoteMirror::Fetch);
                else if (tokens[2] == remoteMirrorPush)
                    remote->setMirror(GitRemoteMirror::Push);
                else
                    THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected Remote Mirror: " + line);
                remotes.push_back(remote);
            }
        CATCH
        return remotes;
    }

    void GitService::AddRemote(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &name, const std::wstring &url, const GitRemoteMirror &mirror)
    {
        TRY
            assert(!IsBlank(name));
            assert(!IsBlank(url));
            std::wstring mirrorStr = L"";
            switch (mirror)
            {
            case GitRemoteMirror::NA:
                break;
            case GitRemoteMirror::Fetch:
                mirrorStr += L" --mirror=fetch";
                break;
            case GitRemoteMirror::Push:
                mirrorStr += L" --mirror=push";
                break;            
            default:
                assert(false);
                break;
            }

            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git remote " + mirrorStr + L" " + name + L" " + url);
        CATCH
    }

    void GitService::RenameRemote(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldName, const std::wstring &newName)
    {
        TRY
            assert(!IsBlank(oldName));
            assert(!IsBlank(newName));
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git rename " + oldName + L" " + newName);
        CATCH
    }
    
    void GitService::RemoveRemote(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &name)
    {
        TRY
            assert(!IsBlank(name));
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git remove " + name);
        CATCH
    }

    void GitService::FetchAll(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git fetch --all");
        CATCH
    }

    void GitService::Pull(const LogConfig *logConfig, const std::wstring &workspace, const GitPullOption *option)
    {
        TRY
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->getIsQuite())
                    optionStr += L" --quiet";

                switch (option->getRecurseSubmodules())
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
                for (const std::wstring &str : option->getRepositories()) {
                    optionStr += L" " + str;
                }
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git pull " + optionStr);
        CATCH
    }

    void GitService::Push(const LogConfig *logConfig, const std::wstring &workspace, const GitPushOption *option)
    {
        TRY
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->getIsForce())
                    optionStr += L" --force";

                if (option->getIsQuite())
                    optionStr += L" --quiet";

                for (const std::wstring &str : option->getRepositories()) {
                    optionStr += L" " + str;
                }
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git push " + optionStr);
        CATCH
    }

    std::wstring getGitLogSearchCriteriaString(const GitLogSearchCriteria *searchCriteria)
    {
        std::wstring optionStr = L"";
        TRY
            if (searchCriteria == nullptr)
                return optionStr;
                
            // Commit Limiting
            if (searchCriteria->getLogCount() > 0)
                optionStr += L" -" + std::to_wstring(searchCriteria->getLogCount());
            
            if (searchCriteria->getSkip() > 0)
                optionStr += L" --skip=" + std::to_wstring(searchCriteria->getSkip());

            if (!IsBlank(searchCriteria->getDateAfter()))
                optionStr += L" --after=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getDateAfter()) + L"\"";

            if (!IsBlank(searchCriteria->getDateBefore()))
                optionStr += L" --before=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getDateBefore()) + L"\"";
            
            if (!IsBlank(searchCriteria->getAuthor()))
                optionStr += L" --author=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getAuthor()) + L"\"";

            if (!IsBlank(searchCriteria->getCommitter()))
                optionStr += L" --committer=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getCommitter()) + L"\"";

            if (!IsBlank(searchCriteria->getGrep()))
                optionStr += L" --grep=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getGrep()) + L"\"";
            
            if (searchCriteria->getIsGrepAllMatch())
                optionStr += L" --all-match";
            
            if (searchCriteria->getIsGrepInvertGrep())
                optionStr += L" --invert-grep";
            
            if (searchCriteria->getIsPatternRegexpIgnoreCase())
                optionStr += L" --regexp-ignore-case";

            if (searchCriteria->getIsPatternExtendedRegexp())
                optionStr += L" --extended-regexp";

            if (searchCriteria->getIsPatternFixedStrings())
                optionStr += L" --fixed-strings";

            if (searchCriteria->getIsMerges())
                optionStr += L" --merges";

            if (searchCriteria->getIsNoMerges())
                optionStr += L" --no-merges";

            if (searchCriteria->getMinParents() >= 0)
                optionStr += L" --min-parents=" + std::to_wstring(searchCriteria->getMinParents());
            
            if (searchCriteria->getMaxParents() >= 0)
                optionStr += L" --max-parents=" + std::to_wstring(searchCriteria->getMaxParents());
            
            if (searchCriteria->getIsFirstParent())
                optionStr += L" --first-parent";

            if (searchCriteria->getIsAll())
                optionStr += L" --all";

            if (searchCriteria->getIsAllBranches())
                optionStr += L" --branches";
            else if (!IsBlank(searchCriteria->getBranches()))
                optionStr += L" --branches=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getBranches()) + L"\"";
            
            if (searchCriteria->getIsAllTags())
                optionStr += L" --tags";
            else if (!IsBlank(searchCriteria->getTags()))
                optionStr += L" --tags=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getTags()) + L"\"";
            
            if (searchCriteria->getIsAllRemotes())
                optionStr += L" --remotes";
            else if (!IsBlank(searchCriteria->getRemotes()))
                optionStr += L" --remotes=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getRemotes()) + L"\"";
            
            if (searchCriteria->getIsAllGlob())
                optionStr += L" --glob";
            else if (!IsBlank(searchCriteria->getGlob()))
                optionStr += L" --glob=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getGlob()) + L"\"";
            
            if (!IsBlank(searchCriteria->getExcludeGlob()))
                optionStr += L" --exclude=\"" + getEscapeString(EscapeStringType::DoubleQuote, searchCriteria->getExcludeGlob()) + L"\"";
            
            // History Simplification
            if (searchCriteria->getIsSimplifyByDecoration())
                optionStr += L" --simplify-by-decoration";

            if (searchCriteria->getIsShowPulls())
                optionStr += L" --show-pulls";

            if (searchCriteria->getIsFullHistory())
                optionStr += L" --full-history";
            
            if (searchCriteria->getIsSparse())
                optionStr += L" --sparse";

            if (searchCriteria->getIsSimplifyMerges())
                optionStr += L" --simplify-merges";

            if (!IsBlank(searchCriteria->getAncestryPath()))
                optionStr += L" --ancestry-path=" + searchCriteria->getAncestryPath();
            
            switch (searchCriteria->getOrderBy())
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
            if (!IsBlank(searchCriteria->getRevisionFrom()) || !IsBlank(searchCriteria->getRevisionTo())) {
                std::wstring revisionStr = L"";
                if (!IsBlank(searchCriteria->getRevisionFrom()))
                    revisionStr += searchCriteria->getRevisionFrom();
                revisionStr += L"..";
                if (!IsBlank(searchCriteria->getRevisionTo()))
                    revisionStr += searchCriteria->getRevisionTo();
                optionStr += L" " + revisionStr;
            }

            // path must be at the end            
            if (!searchCriteria->getPaths().empty()) {
                optionStr += L" --";
                for (const std::wstring &path : searchCriteria->getPaths()) {
                    optionStr += L" " + path;
                }
            }
        CATCH
        return optionStr;
    }

    std::vector<std::shared_ptr<GitLog>> GitService::ParseGitLogGraph(const std::wstring &str)
    {
        std::vector<std::shared_ptr<GitLog>> logs;
        TRY
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                size_t pos = Find(line, L"*");
                if (pos == std::wstring::npos)
                    continue;
                auto log = std::make_shared<GitLog>();
                if (pos > 0) {
                    log->setColumnIndex((size_t)floor(pos/2));
                } else
                    log->setColumnIndex(0);
                std::wregex pattern(L"\\(([^)]+)\\)");
                std::wsmatch match;
                std::wstring::const_iterator searchStart(line.cbegin());
                size_t cnt = 0;
                while (std::regex_search(searchStart, line.cend(), match, pattern)) {
                    switch (cnt)
                    {
                    case 0:
                        log->setHashID(match[1]);
                        break;
                    case 1:
                        log->setAbbreviatedHashID(match[1]);
                        break;
                    case 2:
                        log->setTreeHashID(match[1]);
                        break;
                    case 3:
                        log->setAbbreviatedTreeHashID(match[1]);
                        break;
                    case 4: {
                        std::wstring parentID = match[1];
                        std::vector<std::wstring> tokens = SplitString(parentID, { L" " });
                        for (std::wstring token : tokens) {
                            if (IsBlank(token))
                                continue;
                            Trim(token);
                            log->insertParentHashIDs(token);
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
                            log->insertAbbreviatedParentHashIDs(token);
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
        return logs;
    }

    time_t GitService::ParseGitLogDatetime(const std::wstring &datimeStr)
    {
        time_t time = -1;
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
                    if (!IsBlank(log->getHashID()) && log->getHashID() != tokens.at(0))
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"HashID " + log->getHashID() + L" + not match: " + str);
                    log->setHashID(tokens.at(0));
                    if (Find(tmpLine, L"(") != std::wstring::npos) {
                        std::wregex pattern(L"\\((.*?)\\)");
                        std::wsmatch matches;
                        if (std::regex_search(tmpLine, matches, pattern)) {
                            std::wstring tagsStr = matches[1].str();
                            Trim(tagsStr);
                            if (IsStartWith(tagsStr, headerPrefix)) {
                                log->setIsHead(true);
                                tagsStr = tagsStr.substr(headerPrefix.length());
                                Trim(tagsStr);
                            } else
                                log->setIsHead(false);

                            std::vector<std::wstring> tokens = SplitString(tagsStr, { L"," });
                            for (std::wstring token : tokens) {
                                Trim(token);
                                if (IsStartWith(token, tagPrefix)) {
                                    token = token.substr(tagPrefix.length());
                                    Trim(token);
                                    log->insertTags(token);
                                } else {
                                    log->insertBranches(token);
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
                    log->setAuthor(name);
                    //email
                    std::wregex pattern(L"<(.+?)>");
                    std::wsmatch matches;
                    if (std::regex_search(tmpLine, matches, pattern)) {
                        std::wstring email = matches[1].str();
                        Trim(email);
                        log->setAuthorEmail(email);
                    }
                } else if (IsStartWith(line, authorDatePrefix)) {
                    std::wstring tmpLine = line.substr(authorDatePrefix.length());
                    Trim(tmpLine);
                    log->setAuthorDate(GitService::ParseGitLogDatetime(tmpLine));
                    log->setAuthorDateStr(tmpLine);
                } else if (IsStartWith(line, commitPrefix)) {
                    std::wstring tmpLine = line.substr(commitPrefix.length());
                    Trim(tmpLine);
                    // name
                    std::wstring name = tmpLine.substr(0, Find(tmpLine, L"<") - 1);
                    Trim(name);
                    log->setCommitter(name);
                    //email
                    std::wregex pattern(L"<(.+?)>");
                    std::wsmatch matches;
                    if (std::regex_search(tmpLine, matches, pattern)) {
                        std::wstring email = matches[1].str();
                        Trim(email);
                        log->setCommitterEmail(email);
                    }
                } else if (IsStartWith(line, commitDatePrefix)) {
                    std::wstring tmpLine = line.substr(commitDatePrefix.length());
                    Trim(tmpLine);
                    log->setCommitDate(GitService::ParseGitLogDatetime(tmpLine));
                    log->setCommitDateStr(tmpLine);
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
            log->setTitle(title);
            log->setMessage(message);
            log->setFullMessage(str);
        CATCH
    }
    
    std::vector<std::shared_ptr<GitLog>> GitService::GetLogs(const LogConfig *logConfig, const std::wstring &workspace, const GitLogSearchCriteria *searchCriteria)
    {
        std::vector<std::shared_ptr<GitLog>> logs;
        TRY
            logs = GitService::ParseGitLogGraph(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git log --graph --oneline --pretty=format:\"(%H)(%h)(%T)(%t)(%P)(%p)\" " + getGitLogSearchCriteriaString(searchCriteria)));
            std::map<std::wstring, std::shared_ptr<GitLog>> logMap;
            for (auto log : logs) {
                logMap.insert({log->getHashID(), log});
            }
            std::wstring logDetail = L"";
            std::wstring currentHashID = L"";
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git log --pretty=fuller" + getGitLogSearchCriteriaString(searchCriteria)));
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
        return logs;
    }

    std::shared_ptr<GitLog> GitService::GetCurrentLog(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            auto searchCriteria = std::make_shared<GitLogSearchCriteria>();
            searchCriteria->setLogCount(1);
            auto logs = GitService::GetLogs(logConfig, workspace, searchCriteria.get());
            if (!logs.empty())
                return std::dynamic_pointer_cast<GitLog>(logs.at(0)->Clone());
        CATCH
        return nullptr;
    }

    // void GitService::GetLog(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &hashID, std::shared_ptr<GitLog> log)
    // {
    //     TRY
    //         assert(!IsBlank(hashID));
    //         GitService::ParseGitLog(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git log --pretty=fuller -n 1 " + hashID), log);
    //     CATCH
    // }

    std::vector<std::wstring> GitService::GetTags(const LogConfig *logConfig, const std::wstring &workspace, const GitTagSearchCriteria *searchCriteria)
    {
        std::vector<std::wstring> tags;
        TRY
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (!IsBlank(searchCriteria->getContains()))
                    optionStr += L" --contains " + searchCriteria->getContains();

                if (!IsBlank(searchCriteria->getNoContains()))
                    optionStr += L" --no-contains " + searchCriteria->getNoContains();

                if (!IsBlank(searchCriteria->getOrderBy()))
                    optionStr += L" --sort=" + searchCriteria->getOrderBy();
            }
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git tag -l"));
            tags.insert(tags.end(), lines.begin(), lines.end());
        CATCH
        return tags;
    }

    // void GitService::GetTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, std::shared_ptr<GitLog> log)
    // {
    //     TRY
    //         assert(!IsBlank(tagName));
    //          std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git show --format=fuller " + tagName));
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

    std::shared_ptr<GitTagCurrentTag> GitService::GetCurrentTag(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            auto currentTag = std::make_shared<GitTagCurrentTag>();
            std::wstring tagStr = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git describe --tags"))[0];
            std::vector<std::wstring> tokens = SplitString(tagStr, { L"-" });
            if (tokens.size() >= 3) {
                try
                {
                    currentTag->setNoOfCommit(std::stoi(tokens[tokens.size() - 2]));
                    currentTag->setHashID(tokens[tokens.size() - 1]);
                    tokens.pop_back();
                    tokens.pop_back();
                    currentTag->setTagName(Concat(tokens, L"-"));
                }
                catch(...)
                {
                    currentTag->setTagName(tagStr);
                }
            } else {
                currentTag->setTagName(tagStr);
            }
            return currentTag;
        CATCH
        return nullptr;
    }

    void GitService::CreateTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, const GitTagCreateTagOption *option)
    {
        TRY
            assert(!IsBlank(tagName));
            std::wstring optionStr = L"";
            std::wstring suffixOptionStr = L"";
            if (option != nullptr) {
                switch (option->getSign())
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
                    assert(!IsBlank(option->getSignLocalUserKeyID()));
                    optionStr += L" -local-user=" + option->getSignLocalUserKeyID();
                    break;
                default:
                    assert(false);
                    break;
                }

                if (option->getIsForce())
                    optionStr += L" -f";
                
                if (!IsBlank(option->getMessage()))
                    optionStr += L" -m \"" + getEscapeString(EscapeStringType::DoubleQuote, option->getMessage()) + L"\"";

                if (!IsBlank(option->getHashID()))
                    suffixOptionStr += L" " + option->getHashID();
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git tag" + optionStr + L" " + tagName + L" " + suffixOptionStr);
        CATCH
    }

    void GitService::Switch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName, bool isForce)
    {
        TRY
            assert(!IsBlank(tagName));
            std::wstring optionStr = isForce ? L" -f" : L"";
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git checkout " + optionStr + L" " + tagName);
        CATCH
    }

    void GitService::SwitchReverse(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git switch -");
        CATCH
    }

    void GitService::DeleteTag(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &tagName)
    {
        TRY
            assert(!IsBlank(tagName));
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git tag -d " + tagName);
        CATCH
    }

    void GitService::ParseGitBranch(const std::wstring &str, std::shared_ptr<GitBranch> branch)
    {
        TRY
            std::wstring tmpStr = str;
            std::wstring checkoutPrefix = L"*";
            if (IsStartWith(tmpStr, checkoutPrefix)) {
                branch->setIsActive(true);
                tmpStr = tmpStr.substr(checkoutPrefix.size());
            }
            Trim(tmpStr);
            std::vector<std::wstring> tokens = SplitString(tmpStr, { L" ", L"\t"});
            if (tokens.size() < 3)
                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected git branch pattern: " + str);
            branch->setName(tokens[0]);
            std::wstring hashID = tokens[1];
            bool isPointTo = hashID == L"->";
            std::wstring title = str.substr(Find(str, hashID) + hashID.size());
            Trim(title);
            if (isPointTo) {
                branch->setPointToBranch(title);
            } else {
                branch->setHashID(hashID);
                branch->setTitle(title);
            }
        CATCH
    }

    // void GitService::GetCurrentBranch(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitBranch> branch)
    // {
    //     TRY
    //         std::wstring str = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch -v");
    //         if (str.empty())
    //             return;
    //         std::vector<std::wstring> lines = SplitStringByLine(str);
    //         if (!lines.empty()) {
    //             ParseGitBranch(lines[0], branch);
    //         }
    //     }
    //}

    std::wstring GitService::GetCurrentBranchName(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            return SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch --show-current"))[0];
        CATCH
        return L"";
    }

    std::vector<std::shared_ptr<GitBranch>> GitService::GetBranches(const LogConfig *logConfig, const std::wstring &workspace)
    {
        std::vector<std::shared_ptr<GitBranch>> branches;
        TRY
            std::wstring str = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch -v --all");
            if (str.empty())
                return branches;
            std::vector<std::wstring> lines = SplitStringByLine(str);
            for (const std::wstring &line : lines) {
                if (SplitString(line, { L" ", L"\t" }).size() > 2) {
                    auto branch = std::make_shared<GitBranch>();
                    ParseGitBranch(line, branch);
                    branches.push_back(branch);
                }
            }
        CATCH
        return branches;
    }

    void GitService::CreateBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &branchName, const GitBranchCreateBranchOption *option)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->getIsForce())
                    optionStr += L" -f";

                switch (option->getTrack())
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

                if (option->getIsRecurseSubmodules())
                    optionStr += L" --recurse-submodules";
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + branchName);
        CATCH
    }

    void GitService::SwitchBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &branchName, const GitBranchSwitchBranchOption *option)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = L"";
            if (option != nullptr) {
                if (option->getIsQuite()) {
                    optionStr += L" -q";
                }
                if (option->getIsDiscardChanges()) {
                    optionStr += L" --discard-changes";
                }
            }
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git switch" + optionStr + L" "+ branchName);
        CATCH
    }

    void GitService::RenameBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            assert(!IsBlank(oldBranchName));
            assert(!IsBlank(newBranchName));
            std::wstring optionStr = isForce ? L" -M" : L" -m";
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + oldBranchName + L" " + newBranchName);
        CATCH
    }

    void GitService::CopyBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &oldBranchName, const std::wstring &newBranchName, bool isForce)
    {
        TRY
            assert(!IsBlank(oldBranchName));
            assert(!IsBlank(newBranchName));
            std::wstring optionStr = isForce ? L" -C" : L" -c";
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + oldBranchName + L" " + newBranchName);
        CATCH
    }        
        
    void GitService::DeleteBranch(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &branchName, bool isForce)
    {
        TRY
            assert(!IsBlank(branchName));
            std::wstring optionStr = isForce ? L" -D" : L" -d";
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git branch" + optionStr + L" " + branchName);
        CATCH
    }

    std::shared_ptr<GitDifferenceSummary> GitService::GetDifferenceSummary(const LogConfig *logConfig, const std::wstring &workspace, const std::vector<std::wstring> &hashIDs)
    {
        TRY
            auto summary = std::make_shared<GitDifferenceSummary>();
            std::vector<std::wstring> lines = SplitStringByLine(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git diff " + Concat(hashIDs, L" ") + L" --numstat"));
            for (const std::wstring &line : lines) {
                std::vector<std::wstring> tokens = SplitString(line, { L"\t" }, { L"\"" }, { L"\"" }, { L"\\" });
                if (tokens.size() < 3)
                    continue;
                summary->insertFiles(tokens.at(2));
                summary->insertAddLineCounts((size_t)stoi(tokens.at(0)));
                summary->insertDeleteLineCounts((size_t)stoi(tokens.at(1)));
            }
            return summary;
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitService::ParseGitDiff(const std::wstring &str)
    {
        TRY
            auto difference = std::make_shared<GitDifference>();
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
                        difference->insertChangedLines(currentText);
                        currentText = L"";
                    }
                    rowCount--;
                } else {
                    if (IsStartWith(line, filePathOldPrefix)) {
                        std::wstring tmpStr = line.substr(filePathOldPrefix.length());
                        Trim(tmpStr);
                        difference->setFilePathOld(tmpStr);
                    } else if (IsStartWith(line, filePathNewPrefix)){
                        std::wstring tmpStr = line.substr(filePathNewPrefix.length());
                        Trim(tmpStr);
                        difference->setFilePathNew(tmpStr);
                    } else if (IsStartWith(line, lineCountPrefix)) {
                        // sample new: @@ -1 +1,2 @@
                        // sample modify: @@ -116,10 +116,10 @@ xxxx
                        // 1. Chop to remove all @@
                        // 2. get all numbers
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

                        difference->insertLineNumberOld((size_t)stoi(lineNumberStr.substr(1)));
                        if (lineCountOld.size() > 1) {
                            std::wstring lineCountStr = lineCountOld[1];
                            Trim(lineCountStr);
                            difference->insertLineCountOld((size_t)stoi(lineCountStr));
                        } else
                            difference->insertLineCountOld((size_t)0);

                        // new
                        std::vector<std::wstring> lineCountNew = SplitString(tokens[1], { L"," });
                        if (!(lineCountNew.size() == 1 || lineCountNew.size() == 2))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        lineNumberStr = lineCountNew[0];
                        Trim(lineNumberStr);
                        if (!(IsStartWith(lineNumberStr, L"+") && lineNumberStr.length() > 1))
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Unexpected pattern: " + line);

                        difference->insertLineNumberNew((size_t)stoi(lineNumberStr.substr(1)));
                        if (lineCountNew.size() > 1) {
                            std::wstring lineCountStr = lineCountNew[1];
                            Trim(lineCountStr);
                            difference->insertLineCountNew((size_t)stoi(lineCountStr));
                        } else
                            difference->insertLineCountNew((size_t)0);

                        currentItem = difference->getLineCountNew().size() - 1;
                        rowCount = std::max(difference->getLineCountOld()[currentItem], difference->getLineCountNew()[currentItem]);
                    }
                }
            }
            return difference;
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitService::GetDifferenceIndexFile(const LogConfig *logConfig, const std::wstring &workspace, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            auto diff = std::make_shared<GitDifference>();
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->getNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->getNoOfLines());
                
                if (!searchCriteria->getHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->getHashIDs(), L" ");
            }
            return ParseGitDiff(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git diff --cached" + optionStr));
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitService::GetDifferenceWorkingFile(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->getNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->getNoOfLines());
                
                if (!searchCriteria->getHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->getHashIDs(), L" ");
            }
            return ParseGitDiff(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git diff" + optionStr + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""));
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitService::GetDifferenceFile(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->getNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->getNoOfLines());
                
                if (!searchCriteria->getHashIDs().empty())
                    optionStr += L" " + Concat(searchCriteria->getHashIDs(), L" ");
            }
            return ParseGitDiff(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git diff HEAD" + optionStr + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""));
        CATCH
        return nullptr;
    }

    std::shared_ptr<GitDifference> GitService::GetDifferenceCommit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &fromHashID, const std::wstring &toHashID, const std::wstring &filePath, const GitDifferentSearchCriteria *searchCriteria)
    {
        TRY
            assert(!IsBlank(filePath));
            std::wstring optionStr = L"";
            if (searchCriteria != nullptr) {
                if (searchCriteria->getNoOfLines() > -1)
                    optionStr += L" --unified=" + std::to_wstring(searchCriteria->getNoOfLines());
            }
            return ParseGitDiff(ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git diff" + optionStr + L" " + fromHashID + L"..." + toHashID + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\""));
        CATCH
        return nullptr;
    }

    void GitService::Stage(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git add \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }
    void GitService::StageAll(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git add *");
        CATCH
    }

    void GitService::Unstage(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git reset \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::UnstageAll(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git reset");
        CATCH
    }

    void GitService::Commit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &command)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git commit -m \"" + getEscapeString(EscapeStringType::DoubleQuote, command) + L"\"");
        CATCH
    }

    void GitService::Amend(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git commit --amend");
        CATCH  
    }

    void GitService::DiscardFile(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git checkout \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::RestoreFileToParticularCommit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &filePath, const std::wstring &hashID)
    {
        TRY
            assert(!IsBlank(hashID));
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git restore --source=" + hashID + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, filePath) + L"\"");
        CATCH
    }

    void GitService::ResetCommit(const LogConfig *logConfig, const std::wstring &workspace,  const GitResetMode &resetMode, const std::wstring &hashID)
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
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git reset " + modeStr + hashID);
        CATCH
    }

    void GitService::ReverseCommit(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &hashID)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git revert " + hashID);
        CATCH
    }

    bool GitService::IsConfigExists(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key)
    {
        bool result = false;
        try {
            GitService::GetConfig(logConfig, workspace, key);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;
    }

    void GitService::GetConfig(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitConfig> config)
    {
        TRY
            GitService::GetLocalConfig(logConfig, workspace, config);
            auto globalResult = std::make_shared<GitConfig>();
            GitService::GetGlobalConfig(logConfig, globalResult);
            if (config->getUserName().empty())
                config->setUserName(globalResult->getUserName());
            if (config->getUserEmail().empty())
                config->setUserEmail(globalResult->getUserEmail());
            for (auto tmpConfig : globalResult->getConfigs()) {
                if (config->getConfigs().find(tmpConfig.first) != config->getConfigs().end())
                    config->insertConfigsAtKey(tmpConfig.first, tmpConfig.second);
            }
        CATCH
    }

    std::wstring GitService::GetConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key)
    {
        std::wstring result = L"";
        TRY
            try {
                result = GitService::GetLocalConfig(logConfig, workspace, key);           
            } catch (...) {
                result = L"";
            }
            if (result.empty())
                result = GitService::GetGlobalConfig(logConfig, key);
        CATCH
        return result;
    }

    std::wstring GitService::GetUserName(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            return GitService::GetConfig(logConfig, workspace, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";        
    }
    
    std::wstring GitService::GetUserEmail(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            return GitService::GetConfig(logConfig, workspace, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
    }

    bool GitService::IsGlobalConfigExists(const LogConfig *logConfig, const std::wstring &key)
    {
        bool result = false;
        try {
            auto config = std::make_shared<GitConfig>();
            GitService::GetConfig(logConfig, key, config);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;        
    }

    void GitService::GetGlobalConfig(const LogConfig *logConfig, std::shared_ptr<GitConfig> config)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, L"git config --global --list");
            if (!IsBlank(cmdResult)) {
                auto element = std::make_shared<Config>();
                auto reader = std::make_unique<ConfigBuilder>();
                reader->Deserialize(cmdResult, element);
                for (auto it : element->getConfigs()) {
                    if (!element->isValue(it.first))
                        continue;

                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->setUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->setUserEmail(it.second);
                    }
                    config->insertConfigsAtKey(it.first, it.second);
                }

            }
        CATCH
    }

    std::wstring GitService::GetGlobalConfig(const LogConfig *logConfig, const std::wstring &key)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, L"git config --global " + key);
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        CATCH
        return L"";
    }
    
    void GitService::SetGlobalConfig(const LogConfig *logConfig, const std::wstring &key, const std::wstring &value)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, L"git config --global " + key + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        CATCH
    }

    std::wstring GitService::GetGlobalUserName(const LogConfig *logConfig)
    {
        TRY
            return GitService::GetGlobalConfig(logConfig, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";
    }

    void GitService::SetGlobalUserName(const LogConfig *logConfig, const std::wstring &value)
    {
        TRY
            GitService::SetGlobalConfig(logConfig, GIT_CONFIG_USER_NAME, value);
        CATCH
    }

    std::wstring GitService::GetGlobalUserEmail(const LogConfig *logConfig)
    {
        TRY
            return GitService::GetGlobalConfig(logConfig, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
    }

    void GitService::SetGlobalUserEmail(const LogConfig *logConfig, const std::wstring &value)
    {
        TRY
            GitService::SetGlobalConfig(logConfig, GIT_CONFIG_USER_EMAIL, value);
        CATCH
    }

    bool GitService::IsLocalConfigExists(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key)
    {
        bool result = false;
        try {
            GitService::GetLocalConfig(logConfig, workspace, key);
            result = true;
        } catch (...) {
            result = false;
        }
        return result;       
    }

    void GitService::GetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, std::shared_ptr<GitConfig> config)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git config --local --list");
            if (!IsBlank(cmdResult)) {
                auto element = std::make_shared<Config>();
                auto reader = std::make_unique<ConfigBuilder>();

                reader->Deserialize(cmdResult, element);
                for (auto it : element->getConfigs()) {
                    if (!element->isValue(it.first))
                        continue;
                    if (it.first == GIT_CONFIG_USER_NAME) {
                        config->setUserName(it.second);
                    } else if (it.first == GIT_CONFIG_USER_EMAIL) {
                        config->setUserEmail(it.second);
                    }
                    config->insertConfigsAtKey(it.first, it.second);
                }
            }
        CATCH
    }

    std::wstring GitService::GetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key)
    {
        TRY
            std::wstring cmdResult = ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git config --local " + key);
            return IsBlank(cmdResult) ? L"" : SplitStringByLine(cmdResult).at(0);
        CATCH
        return L"";
    }

    void GitService::SetLocalConfig(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &key, const std::wstring &value)
    {
        TRY
            ProcessService::Execute(logConfig, GIT_LOG_ID, workspace, L"git config --local " + key + L" \"" + getEscapeString(EscapeStringType::DoubleQuote, value) + L"\"");
        CATCH
    }

    std::wstring GitService::GetLocalUserName(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            return GitService::GetLocalConfig(logConfig, workspace, GIT_CONFIG_USER_NAME);
        CATCH
        return L"";
    }

    void GitService::SetLocalUserName(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &value)
    {
        TRY
            GitService::SetLocalConfig(logConfig, workspace, GIT_CONFIG_USER_NAME, value);
        CATCH
    }

    std::wstring GitService::GetLocalUserEmail(const LogConfig *logConfig, const std::wstring &workspace)
    {
        TRY
            return GitService::GetLocalConfig(logConfig, workspace, GIT_CONFIG_USER_EMAIL);
        CATCH
        return L"";
    }

    void GitService::SetLocalUserEmail(const LogConfig *logConfig, const std::wstring &workspace, const std::wstring &value)
    {
        TRY
            GitService::SetLocalConfig(logConfig, workspace, GIT_CONFIG_USER_EMAIL, value);
        CATCH
    }
}