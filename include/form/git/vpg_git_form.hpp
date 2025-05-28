// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include <memory>

#include "base_form.hpp"
#include "base_json_object.hpp"
#include "class_macro.hpp"
#include "git_manager.hpp"
#include "i_document.hpp"
#include "i_result.hpp"
#include "json.hpp"
#include "object_type.hpp"
#include "vpg_git_log.hpp"

// <vcc:customHeader sync="RESERVE" gen="RESERVE">
// </vcc:customHeader>

using namespace vcc;

class VPGGitForm : public BaseForm, public BaseJsonObject
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

        virtual std::shared_ptr<Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;

        virtual void InitializeComponents() override;

        virtual std::shared_ptr<IResult> DoAction(const int64_t &formProperty, std::shared_ptr<IObject> argument) override;

        // <vcc:customVPGGitFormPublicFunctions sync="RESERVE" gen="RESERVE">
        // </vcc:customVPGGitFormPublicFunctions>
};
