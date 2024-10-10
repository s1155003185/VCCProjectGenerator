#pragma once

#include "base_form.hpp"
#include "object_type.hpp"

using namespace vcc;

class VPGGitForm : public BaseForm<VPGGitForm>
{
    public:
        VPGGitForm(std::shared_ptr<LogProperty> logProperty) : BaseForm(logProperty, ObjectType::GitForm) {}
        virtual ~VPGGitForm() {}
        
        void OnInitialize() const override;
};
