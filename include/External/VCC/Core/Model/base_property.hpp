#pragma once
#include "i_property.hpp"

#include "class_macro.hpp"

namespace vcc
{
    class BaseProperty : public IProperty
    {
        protected:
            BaseProperty() {}
            ~BaseProperty() {}
    };
}
