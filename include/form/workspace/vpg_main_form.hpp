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

class VPGMainFormAddWorkspace : public BaseAction
{
    // <vcc:customVPGMainFormAddWorkspaceProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormAddWorkspaceProperties>

    private:
        // <vcc:customVPGMainFormAddWorkspacePrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspacePrivateFunctions>

    protected:
        virtual std::wstring GetRedoMessageStart() const override;
        virtual std::wstring GetRedoMessageComplete() const override;
        virtual std::wstring GetUndoMessageStart() const override;
        virtual std::wstring GetUndoMessageComplete() const override;

        virtual void OnRedo() const override;
        virtual void OnUndo() const override;

        // <vcc:customVPGMainFormAddWorkspaceProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceProtectedFunctions>

    public:
        VPGMainFormAddWorkspace() : BaseAction() {}
        VPGMainFormAddWorkspace(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);
        ~VPGMainFormAddWorkspace() {}

        // <vcc:customVPGMainFormAddWorkspacePublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspacePublicFunctions>
};

class VPGMainFormDeleteWorkspace : public BaseAction
{
    // <vcc:customVPGMainFormDeleteWorkspaceProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormDeleteWorkspaceProperties>

    private:
        // <vcc:customVPGMainFormDeleteWorkspacePrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspacePrivateFunctions>

    protected:
        virtual std::wstring GetRedoMessageStart() const override;
        virtual std::wstring GetRedoMessageComplete() const override;
        virtual std::wstring GetUndoMessageStart() const override;
        virtual std::wstring GetUndoMessageComplete() const override;

        virtual void OnRedo() const override;
        virtual void OnUndo() const override;

        // <vcc:customVPGMainFormDeleteWorkspaceProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceProtectedFunctions>

    public:
        VPGMainFormDeleteWorkspace() : BaseAction() {}
        VPGMainFormDeleteWorkspace(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);
        ~VPGMainFormDeleteWorkspace() {}

        // <vcc:customVPGMainFormDeleteWorkspacePublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspacePublicFunctions>
};

class VPGMainForm : public BaseForm
{
    VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    ACTION(AddWorkspace)
    ACTION(DeleteWorkspace)

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
