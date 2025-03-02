#pragma once

// @@ActionArgument
enum class VPGMainFormAddWorkspaceFormArgumentProperty
{
    Name // GETSET(std::wstring, Name, L"")
};

// @@ActionArgument
enum class VPGMainFormDeleteWorkspaceFormArgumentProperty
{
    WorkspaceForm // GETSET_SPTR_NULL(VPGWorkspaceForm, WorkspaceForm)
};

// @@ActionArgument
enum class VPGMainFormRenameWorkspaceFormArgumentProperty
{
    WorkspaceForm // GETSET_SPTR_NULL(VPGWorkspaceForm, WorkspaceForm)
    , NewName // GETSET(std::wstring, NewName, L"")
};

// @@Form @@Json
enum class VPGMainFormProperty
{
    WorkspaceForms // VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    , Initialize // ACTION(Initialize) @@NoHistory
    , AddWorkspaceForm // ACTION_WITH_ARG_SPTR(AddWorkspaceForm, VPGMainFormAddWorkspaceFormArgument) @@NoHistory
    , DeleteWorkspaceForm // ACTION_WITH_ARG_SPTR(DeleteWorkspaceForm, VPGMainFormDeleteWorkspaceFormArgument) @@NoHistory
    , RenameWorkspaceForm // ACTION_WITH_ARG_SPTR(RenameWorkspaceForm, VPGMainFormRenameWorkspaceFormArgument) @@NoHistory
};