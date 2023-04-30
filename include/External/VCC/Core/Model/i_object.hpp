#pragma once
#include <string>

namespace vcc
{
    class IObject
    {
        protected:
            IObject() {}
            ~IObject() {}

        public:
            virtual std::wstring GetKey() = 0;
    };
}
