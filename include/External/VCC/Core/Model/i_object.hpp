#pragma once
#include <memory>
#include <string>

namespace vcc
{
    class IObject
    {
        protected:
            IObject() {}
            virtual ~IObject() {}

        public:
            virtual std::shared_ptr<IObject> Clone() const = 0;
    };
}
