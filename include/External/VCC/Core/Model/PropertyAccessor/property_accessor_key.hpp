#pragma once

#include "base_type_union.hpp"

namespace vcc
{
    class PropertyAccessorKey : public BaseTypeUnion
    {
        protected:
            PropertyAccessorKey() {}
            virtual ~PropertyAccessorKey() {}
    };
}
