#include "vpg_git_form.hpp"

bool VPGGitForm::IsClosable() const
{
    return true;
}

void VPGGitForm::OnInitialize() const
{
    this->_Log = nullptr;
}
