#pragma once

namespace vcc
{
    class IFactory
    {
        protected:
            IFactory() = default;
            virtual ~IFactory() {}
    };
}
