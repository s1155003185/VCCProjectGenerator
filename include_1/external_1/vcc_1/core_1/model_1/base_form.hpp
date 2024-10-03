#pragma once

#include "i_form.hpp"
#include "base_form.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"

#include "form_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseForm : public IForm, public BaseObject<Derived>
    {
        GETSET_SPTR_NULL(LogProperty, LogProperty);
        GETSET(FormType, Type, FormType::NA);
        
    protected:
        BaseForm() = default;
        BaseForm(std::shared_ptr<LogProperty> logProperty) { this->_LogProperty = logProperty; }
        BaseForm(std::shared_ptr<LogProperty> logProperty, FormType type) : BaseForm(logProperty) { this->_Type = type; }
        virtual ~BaseForm() {}
    };
}
