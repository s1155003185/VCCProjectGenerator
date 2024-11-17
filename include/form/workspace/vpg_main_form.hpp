// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include "base_form.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"
#include "vpg_workspace_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGMainForm : public BaseForm
{
    VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)

    public:
        VPGMainForm() : BaseForm(ObjectType::MainForm) {}
        virtual ~VPGMainForm() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            std::shared_ptr<VPGMainForm> obj = std::make_shared<VPGMainForm>(*this);
            obj->CloneWorkspaceForms(this->_WorkspaceForms);
            return obj;
        }

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // Initialize
        void OnInitialize() const override;
        // Close
        bool IsClosable() const override;
        // </vcc:custom>
};
