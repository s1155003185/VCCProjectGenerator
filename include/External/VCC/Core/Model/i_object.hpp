#pragma once
#include <memory>
#include <string>

namespace vcc
{
    template <typename Derived>
    class IObject
    {
        protected:
            IObject() {}
            ~IObject() {}

        public:
            virtual std::shared_ptr<Derived> Clone() = 0;
    };
}
