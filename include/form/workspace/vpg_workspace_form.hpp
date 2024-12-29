// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <string>

#include "base_form.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"
#include "vpg_git_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGWorkspaceForm : public BaseForm
{
    GETSET(std::wstring, Name, L"")
    VECTOR_SPTR(VPGGitForm, GitForms)

    // <vcc:customVPGWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGWorkspaceFormProperties>

    private:
        // <vcc:customVPGWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGWorkspaceFormPrivateFunctions>

    protected:
        // <vcc:customVPGWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGWorkspaceFormProtectedFunctions>

    public:
        VPGWorkspaceForm();
        virtual ~VPGWorkspaceForm() {}

        virtual std::shared_ptr<IObject> Clone() const override;

        virtual void InitializeComponents() const override;

        virtual void DoAction(const int64_t &formProperty) override;

        // <vcc:customVPGWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGWorkspaceFormPublicFunctions>
};
