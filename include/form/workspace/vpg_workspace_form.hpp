// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include "base_form.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"
#include "vpg_git_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGWorkspaceForm : public BaseForm
{
    VECTOR_SPTR(VPGGitForm, GitForms)

    public:
        VPGWorkspaceForm() : BaseForm(ObjectType::WorkspaceForm) {}
        virtual ~VPGWorkspaceForm() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            std::shared_ptr<VPGWorkspaceForm> obj = std::make_shared<VPGWorkspaceForm>(*this);
            obj->CloneGitForms(this->_GitForms);
            return obj;
        }

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // Initialize
        void OnInitialize() const override;
        // Close
        bool IsClosable() const override;
        // </vcc:custom>
};
