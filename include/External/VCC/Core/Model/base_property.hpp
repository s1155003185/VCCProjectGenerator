#pragma once
#include "i_property.hpp"

#include "base_object.hpp"
#include "class_macro.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseProperty : public IProperty, public BaseObject<Derived>
    {
        protected:
            BaseProperty() {}
            ~BaseProperty() {}
    };
}
