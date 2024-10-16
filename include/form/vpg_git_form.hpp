#pragma once

#include "base_form.hpp"
#include "base_object.hpp"
#include "object_type.hpp"
#include "git_service.hpp"
#include "vpg_git_log.hpp"

using namespace vcc;

class VPGGitForm : public BaseForm<VPGGitForm>
{
    GETSET_SPTR_NULL(VPGGitLog, Log);
    public:
        VPGGitForm(std::shared_ptr<LogConfig> logConfig) : BaseForm(logConfig, ObjectType::GitForm) {}
        virtual ~VPGGitForm() {}
        
        bool IsClosable() const override;
        
        void OnInitialize() const override;
        
};
