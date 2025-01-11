// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>
#include <string>

#include "base_action.hpp"
#include "base_form.hpp"
#include "class_macro.hpp"
#include "i_object.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "vpg_workspace_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGMainFormAddWorkspaceForm : public BaseAction
{
    // <vcc:customVPGMainFormAddWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormAddWorkspaceFormProperties>

    private:
        // <vcc:customVPGMainFormAddWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormPrivateFunctions>

    protected:
        virtual std::wstring GetRedoMessageStart() const override;
        virtual std::wstring GetRedoMessageComplete() const override;

        virtual void OnRedo() override;

        // <vcc:customVPGMainFormAddWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormProtectedFunctions>

    public:
        VPGMainFormAddWorkspaceForm() : BaseAction() {}
        VPGMainFormAddWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);
        ~VPGMainFormAddWorkspaceForm() {}

        // <vcc:customVPGMainFormAddWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormPublicFunctions>
};

class VPGMainFormDeleteWorkspaceForm : public BaseAction
{
    // <vcc:customVPGMainFormDeleteWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormDeleteWorkspaceFormProperties>

    private:
        // <vcc:customVPGMainFormDeleteWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormPrivateFunctions>

    protected:
        virtual std::wstring GetRedoMessageStart() const override;
        virtual std::wstring GetRedoMessageComplete() const override;

        virtual void OnRedo() override;

        // <vcc:customVPGMainFormDeleteWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormProtectedFunctions>

    public:
        VPGMainFormDeleteWorkspaceForm() : BaseAction() {}
        VPGMainFormDeleteWorkspaceForm(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);
        ~VPGMainFormDeleteWorkspaceForm() {}

        // <vcc:customVPGMainFormDeleteWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormPublicFunctions>
};

class VPGMainForm : public BaseForm
{
    VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    GETSET_SPTR_NULL(VPGWorkspaceForm, CurrentWorkspaceForm)
    ACTION(AddWorkspaceForm)
    ACTION(DeleteWorkspaceForm)

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

        virtual void DoAction(const int64_t &formProperty) override;

        // <vcc:customVPGMainFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormPublicFunctions>
};
