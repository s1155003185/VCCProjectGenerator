#pragma once
#include "i_factory.hpp"

namespace vcc
{
    class BaseFactory : public IFactory
    {
        protected:
            BaseFactory() = default;
            virtual ~BaseFactory() {}
    };
}
