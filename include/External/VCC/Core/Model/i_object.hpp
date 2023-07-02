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
            virtual std::wstring ToString() = 0;
            virtual std::shared_ptr<IObject> Clone() = 0;
    };
}
