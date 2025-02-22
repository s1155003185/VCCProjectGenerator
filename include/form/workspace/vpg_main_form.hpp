// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>
#include <string>

#include "base_action.hpp"
#include "base_form.hpp"
#include "base_json_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "i_object.hpp"
#include "json.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "vpg_workspace_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGMainFormInitialize : public BaseAction
{
    // <vcc:customVPGMainFormInitializeProperties sync="RESERVE" gen="RESERVE">
    // </vcc:customVPGMainFormInitializeProperties>

    private:
        // <vcc:customVPGMainFormInitializePrivateFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializePrivateFunctions>

    protected:
        virtual std::wstring GetRedoMessageStart() const override;
        virtual std::wstring GetRedoMessageComplete() const override;

        virtual void OnRedo() override;

        // <vcc:customVPGMainFormInitializeProtectedFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializeProtectedFunctions>

    public:
        VPGMainFormInitialize() : BaseAction() {}
        VPGMainFormInitialize(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);
        ~VPGMainFormInitialize() {}

        // <vcc:customVPGMainFormInitializePublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormInitializePublicFunctions>
};

class VPGMainForm : public BaseForm, public BaseJsonObject
{
    VECTOR_SPTR(VPGWorkspaceForm, WorkspaceForms)
    ACTION(Initialize)

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

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;

        virtual void InitializeComponents() const override;

        virtual void DoAction(const int64_t &formProperty) override;

        // <vcc:customVPGMainFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGMainFormPublicFunctions>
};
