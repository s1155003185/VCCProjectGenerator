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

    // <vcc:customVPGMainFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormProperties>

    private:
        // <vcc:customVPGMainFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormPrivateFunctions>

    protected:
        // <vcc:customVPGMainFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormProtectedFunctions>

    public:
        VPGMainForm();
        virtual ~VPGMainForm() {}

        virtual std::shared_ptr<IObject> Clone() const override;

        virtual void InitializeComponents() const override;

        virtual void DoAction(const int64_t &formProperty) const override;

        // <vcc:customVPGMainFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormPublicFunctions>
};
