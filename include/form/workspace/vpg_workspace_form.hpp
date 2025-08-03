// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>
#include <string>

#include "base_form.hpp"
#include "base_json_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "i_result.hpp"
#include "json.hpp"
#include "object_type.hpp"
#include "vpg_git_form.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

class VPGWorkspaceForm : public vcc::BaseForm, public vcc::BaseJsonObject
{
    GETSET(std::wstring, Name, L"")
    GETSET(int64_t, TabOrder, -1)
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

        virtual std::shared_ptr<vcc::IObject> clone() const override;

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void deserializeJson(std::shared_ptr<vcc::IDocument> document) override;

        virtual void initializeComponents() override;

        virtual std::shared_ptr<vcc::IResult> doAction(const int64_t &formProperty, std::shared_ptr<vcc::IObject> argument) override;

        // <vcc:customVPGWorkspaceFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGWorkspaceFormPublicFunctions>
};
