#pragma once

// @@Form
enum class VPGMainFormProperty
{
    WorkspaceForms // VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    , CurrentWorkspaceForm // GETSET_SPTR_NULL(VPGWorkspaceForm, CurrentWorkspaceForm)
    , AddWorkspaceForm // ACTION(AddWorkspaceForm) @@NoHistory
    , DeleteWorkspaceForm // ACTION(DeleteWorkspaceForm) @@NoHistory
};