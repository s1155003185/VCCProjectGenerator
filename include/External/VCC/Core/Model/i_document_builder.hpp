#pragma once

#include <memory>

namespace vcc
{

    class IDocument
    {
    protected:
        IDocument() {}
        ~IDocument() {}

    public:
        virtual std::shared_ptr<IDocument> Serialize() = 0;
        virtual Deserialize() = 0
    };
}
