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
            virtual std::shared_ptr<IObject> Clone() = 0;
    };
}
