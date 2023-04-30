#pragma once
#include "i_property.hpp"

#include "class_macro.hpp"

namespace vcc
{
    class BaseProperty : public IProperty
    {
        THREAD_SAFE
        protected:
            BaseProperty() {}
            ~BaseProperty() {}
    };
}
