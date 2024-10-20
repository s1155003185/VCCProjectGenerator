// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

#include "base_form.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"
#include "vpg_git_log.hpp"

// <vcc:customHeader sync="SKIP" gen="SKIP">
// </vcc:customHeader>

using namespace vcc;

class VPGGitForm : public BaseForm<VPGGitForm>
{
    GETSET_SPTR_NULL(VPGGitLog, Log)

    public:
        VPGGitForm(std::shared_ptr<LogConfig> logConfig = nullptr) : BaseForm(logConfig, ObjectType::GitForm) {}
        virtual ~VPGGitForm() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            std::shared_ptr<VPGGitForm> obj = std::make_shared<VPGGitForm>(*this);
            obj->CloneLog(this->_Log);
            return obj;
        }

        // <vcc:custom sync="RESERVE" gen="RESERVE">
        // Initialize
        void OnInitialize() const override;
        // Close
        bool IsClosable() const override;
        // </vcc:custom>
};
