#pragma once
#include "base_key.hpp"

namespace vcc
{
    class PropertyAccessorKey : public BaseTypeUnion
    {
        protected:
            PropertyAccessorKey() {}
            ~PropertyAccessorKey() {}
    };
}
