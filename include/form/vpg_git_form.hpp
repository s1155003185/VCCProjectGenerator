#pragma once

#include "base_form.hpp"
#include "base_object.hpp"
#include "object_type.hpp"
#include "git_service.hpp"

using namespace vcc;

class VPGGitLog : public GitLog
{
    public:
        VPGGitLog() : GitLog()
        {
            _ObjectType = ObjectType::GitLog;
        }
        
        virtual ~VPGGitLog();
};

class VPGGitForm : public BaseForm<VPGGitForm>
{
    GETSET_SPTR_NULL(VPGGitLog, Log);
    public:
        VPGGitForm(std::shared_ptr<LogProperty> logProperty) : BaseForm(logProperty, ObjectType::GitForm) {}
        virtual ~VPGGitForm() {}
        
        void OnInitialize() const override;
};
