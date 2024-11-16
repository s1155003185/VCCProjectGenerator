#pragma once

#include "i_form.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "object_type.hpp"

namespace vcc
{
    class BaseForm : public IForm, public BaseObject
    {
        GETSET_SPTR_NULL(LogConfig, LogConfig);
        
    protected:
        BaseForm() = default;
        BaseForm(std::shared_ptr<LogConfig> logConfig, ObjectType type)
            : BaseObject(type), _LogConfig(logConfig) {}
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
