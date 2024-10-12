#pragma once

#include "i_form.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "object_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseForm : public IForm, public BaseObject<Derived>
    {
        GETSET_SPTR_NULL(LogConfig, LogConfig);
        
    protected:
        BaseForm() = default;
        BaseForm(std::shared_ptr<LogConfig> logProperty, ObjectType type)
            : BaseObject<Derived>(type), _LogConfig(logProperty) {}
        virtual ~BaseForm() = default;

    public:
        bool OnClose(bool isForce = false) const override
        {
            if (!isForce && !IsClosable())
                return false;
            return true;
        }
    };
}
