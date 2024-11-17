#pragma once

#include "i_form.hpp"
#include "base_form.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "object_type.hpp"
#include "state.hpp"

namespace vcc
{
    class BaseForm : public IForm, public BaseObject
    {
        GETSET_SPTR_NULL(LogConfig, LogConfig);
        
    protected:
        mutable State _State = State::Active;

        BaseForm() : BaseForm(ObjectType::NA) {}
        BaseForm(const ObjectType &type) : BaseObject(type) { Initialize(); }
        virtual ~BaseForm() = default;

        virtual void OnInitialize() const override {}
        virtual void OnReload() const override {}
        virtual bool OnIsClosable() const override { return true; }
        virtual bool OnClose() const override { return true; }
    public:
        State GetState() const override;
        bool IsClosable() const override;
        bool IsClosed() const override;

        void Initialize() const override;
        void Reload() const override;

        bool Close(bool isForce = false) const override;
    };
}
