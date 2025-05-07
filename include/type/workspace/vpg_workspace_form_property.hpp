#pragma once

// @@Form @@Json
enum class VPGWorkspaceFormProperty
{
    Name // GETSET(std::wstring, Name, L"")
    , TabOrder // GETSET(int64_t, TabOrder, -1) @@Command("Start From 0")
    , GitForms // VECTOR_SPTR(VPGGitForm, GitForms)
};
