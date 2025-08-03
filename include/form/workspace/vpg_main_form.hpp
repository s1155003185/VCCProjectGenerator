// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>
#include <string>

#include "base_action.hpp"
#include "base_action_argument.hpp"
#include "base_form.hpp"
#include "base_json_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "i_object.hpp"
#include "i_result.hpp"
#include "json.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "vpg_workspace_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

class VPGMainFormAddWorkspaceFormArgument : public vcc::BaseActionArgument
{
    GETSET(std::wstring, Name, L"")

    public:
        VPGMainFormAddWorkspaceFormArgument() : vcc::BaseActionArgument(ObjectType::MainFormAddWorkspaceFormArgument) {}
        virtual ~VPGMainFormAddWorkspaceFormArgument() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGMainFormAddWorkspaceFormArgument>(*this);
        }
};

class VPGMainFormDeleteWorkspaceFormArgument : public vcc::BaseActionArgument
{
    GETSET_SPTR_NULL(VPGWorkspaceForm, WorkspaceForm)

    public:
        VPGMainFormDeleteWorkspaceFormArgument() : vcc::BaseActionArgument(ObjectType::MainFormDeleteWorkspaceFormArgument) {}
        virtual ~VPGMainFormDeleteWorkspaceFormArgument() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            auto obj = std::make_shared<VPGMainFormDeleteWorkspaceFormArgument>(*this);
            obj->cloneWorkspaceForm(this->_WorkspaceForm.get());
            return obj;
        }
};

class VPGMainFormRenameWorkspaceFormArgument : public vcc::BaseActionArgument
{
    GETSET_SPTR_NULL(VPGWorkspaceForm, WorkspaceForm)
    GETSET(std::wstring, NewName, L"")

    public:
        VPGMainFormRenameWorkspaceFormArgument() : vcc::BaseActionArgument(ObjectType::MainFormRenameWorkspaceFormArgument) {}
        virtual ~VPGMainFormRenameWorkspaceFormArgument() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            auto obj = std::make_shared<VPGMainFormRenameWorkspaceFormArgument>(*this);
            obj->cloneWorkspaceForm(this->_WorkspaceForm.get());
            return obj;
        }
};

class VPGMainFormAddWorkspaceForm : public vcc::BaseAction
{
    GETSET_SPTR_NULL(VPGMainFormAddWorkspaceFormArgument, Argument)

    // <vcc:customVPGMainFormAddWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormAddWorkspaceFormProperties>

    private:
        // <vcc:customVPGMainFormAddWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormPrivateFunctions>

    protected:
        virtual std::wstring getRedoMessageStart() const override;
        virtual std::wstring getRedoMessageComplete() const override;

        virtual std::shared_ptr<vcc::IResult> onRedo() override;

        // <vcc:customVPGMainFormAddWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormProtectedFunctions>

    public:
        VPGMainFormAddWorkspaceForm() : vcc::BaseAction() {}
        VPGMainFormAddWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm);
        VPGMainFormAddWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormAddWorkspaceFormArgument> argument);
        ~VPGMainFormAddWorkspaceForm() {}

        // <vcc:customVPGMainFormAddWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormAddWorkspaceFormPublicFunctions>
};

class VPGMainFormDeleteWorkspaceForm : public vcc::BaseAction
{
    GETSET_SPTR_NULL(VPGMainFormDeleteWorkspaceFormArgument, Argument)

    // <vcc:customVPGMainFormDeleteWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormDeleteWorkspaceFormProperties>

    private:
        // <vcc:customVPGMainFormDeleteWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormPrivateFunctions>

    protected:
        virtual std::wstring getRedoMessageStart() const override;
        virtual std::wstring getRedoMessageComplete() const override;

        virtual std::shared_ptr<vcc::IResult> onRedo() override;

        // <vcc:customVPGMainFormDeleteWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormProtectedFunctions>

    public:
        VPGMainFormDeleteWorkspaceForm() : vcc::BaseAction() {}
        VPGMainFormDeleteWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm);
        VPGMainFormDeleteWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormDeleteWorkspaceFormArgument> argument);
        ~VPGMainFormDeleteWorkspaceForm() {}

        // <vcc:customVPGMainFormDeleteWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormDeleteWorkspaceFormPublicFunctions>
};

class VPGMainFormInitialize : public vcc::BaseAction
{
    // <vcc:customVPGMainFormInitializeProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormInitializeProperties>

    private:
        // <vcc:customVPGMainFormInitializePrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializePrivateFunctions>

    protected:
        virtual std::wstring getRedoMessageStart() const override;
        virtual std::wstring getRedoMessageComplete() const override;

        virtual std::shared_ptr<vcc::IResult> onRedo() override;

        // <vcc:customVPGMainFormInitializeProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializeProtectedFunctions>

    public:
        VPGMainFormInitialize() : vcc::BaseAction() {}
        VPGMainFormInitialize(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm);
        ~VPGMainFormInitialize() {}

        // <vcc:customVPGMainFormInitializePublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializePublicFunctions>
};

class VPGMainFormRenameWorkspaceForm : public vcc::BaseAction
{
    GETSET_SPTR_NULL(VPGMainFormRenameWorkspaceFormArgument, Argument)

    // <vcc:customVPGMainFormRenameWorkspaceFormProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormRenameWorkspaceFormProperties>

    private:
        // <vcc:customVPGMainFormRenameWorkspaceFormPrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormRenameWorkspaceFormPrivateFunctions>

    protected:
        virtual std::wstring getRedoMessageStart() const override;
        virtual std::wstring getRedoMessageComplete() const override;

        virtual std::shared_ptr<vcc::IResult> onRedo() override;

        // <vcc:customVPGMainFormRenameWorkspaceFormProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormRenameWorkspaceFormProtectedFunctions>

    public:
        VPGMainFormRenameWorkspaceForm() : vcc::BaseAction() {}
        VPGMainFormRenameWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm);
        VPGMainFormRenameWorkspaceForm(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, std::shared_ptr<VPGMainFormRenameWorkspaceFormArgument> argument);
        ~VPGMainFormRenameWorkspaceForm() {}

        // <vcc:customVPGMainFormRenameWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormRenameWorkspaceFormPublicFunctions>
};

class VPGMainForm : public vcc::BaseForm, public vcc::BaseJsonObject
{
    VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    ACTION(Initialize)
    ACTION_WITH_ARG_SPTR(AddWorkspaceForm, VPGMainFormAddWorkspaceFormArgument)
    ACTION_WITH_ARG_SPTR(DeleteWorkspaceForm, VPGMainFormDeleteWorkspaceFormArgument)
    ACTION_WITH_ARG_SPTR(RenameWorkspaceForm, VPGMainFormRenameWorkspaceFormArgument)

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

        virtual std::shared_ptr<vcc::IObject> clone() const override;

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;

        virtual void initializeComponents() override;

        virtual std::shared_ptr<vcc::IResult> doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> argument) override;

        // <vcc:customVPGMainFormPublicFunctions sync="RESERVE" gen="RESERVE">
        void saveConfig() const;
        // </vcc:customVPGMainFormPublicFunctions>
};
