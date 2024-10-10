#pragma once

#include "base_form.hpp"

using namespace vcc;

class VPGGitForm : public BaseForm<VPGGitForm>
{
    public:
        VPGGitForm(std::shared_ptr<LogProperty> logProperty) : BaseForm(logProperty, FormType::Git) {}
        virtual ~VPGGitForm() {}
        
        void OnInitialize() const override {}
};
