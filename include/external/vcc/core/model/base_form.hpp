#pragma once

#include "i_form.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"
#include "object_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseForm : public IForm, public BaseObject<Derived>
    {
        GETSET_SPTR_NULL(LogProperty, LogProperty);
        
    protected:
        BaseForm() = default;
        BaseForm(std::shared_ptr<LogProperty> logProperty, ObjectType type)
            : BaseObject<Derived>(type), _LogProperty(logProperty) {}
        virtual ~BaseForm() {}
    };
}
