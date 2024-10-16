#pragma once

// @@Inherit{ "Class": "GitLog" }
enum class VPGGitLogProperty
{
    ColumnIndex // GETSET(int64_t, ColumnIndex, 0) @@Inherit
    , HashID // GETSET(std::wstring, HashID, L"") @@Inherit
    , AbbreviatedHashID // GETSET(std::wstring, AbbreviatedHashID, L"") @@Inherit
    , TreeHashID // GETSET(std::wstring, TreeHashID, L"") @@Inherit
    , AbbreviatedTreeHashID // GETSET(std::wstring, AbbreviatedTreeHashID, L"") @@Inherit
    , ParentHashIDs // VECTOR(std::wstring, ParentHashIDs) @@Inherit
    , AbbreviatedParentHashIDs // VECTOR(std::wstring, AbbreviatedParentHashIDs) @@Inherit
    , IsHead // GETSET(bool, IsHead, false) @@Inherit
    , Branches // VECTOR(std::wstring, Branches) @@Inherit
    , Tags // VECTOR(std::wstring, Tags) @@Inherit
    , Author // GETSET(std::wstring, Author, L"") @@Inherit
    , AuthorEmail // GETSET(std::wstring, AuthorEmail, L"") @@Inherit
    , AuthorDate // GETSET(time_t, AuthorDate, -1) @@Inherit
    , AuthorDateStr // GETSET(std::wstring, AuthorDateStr, L"") @@Inherit
    , Committer // GETSET(std::wstring, Committer, L"") @@Inherit
    , CommitterEmail // GETSET(std::wstring, CommitterEmail, L"") @@Inherit
    , CommitDate // GETSET(time_t, CommitDate, -1) @@Inherit
    , CommitDateStr // GETSET(std::wstring, CommitDateStr, L"") @@Inherit
    , Title // GETSET(std::wstring, Title, L"") @@Inherit
    , Message // GETSET(std::wstring, Message, L"") @@Inherit
    , FullMessage // GETSET(std::wstring, FullMessage, L"") @@Inherit
};