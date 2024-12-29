// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include "base_form.hpp"
#include "class_macro.hpp"
#include "git_manager.hpp"
#include "object_type.hpp"
#include "vpg_git_log.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGGitForm : public BaseForm
{
    GETSET_SPTR_NULL(VPGGitLog, Log)
    MANAGER_SPTR_NULL(GitManager, GitManager, _LogConfig)

    // <vcc:customVPGGitFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGGitFormProperties>

    private:
        // <vcc:customVPGGitFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGGitFormPrivateFunctions>

    protected:
        // <vcc:customVPGGitFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGGitFormProtectedFunctions>

    public:
        VPGGitForm();
        virtual ~VPGGitForm() {}

        virtual std::shared_ptr<IObject> Clone() const override;

        virtual void InitializeComponents() const override;

        virtual void DoAction(const int64_t &formProperty) override;

        // <vcc:customVPGGitFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGGitFormPublicFunctions>
};
