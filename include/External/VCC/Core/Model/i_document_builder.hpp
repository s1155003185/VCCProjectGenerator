#pragma once

#include <memory>
#include <string>

namespace vcc
{
    class IDocument;
    class IDocumentBuilder
    {
    protected:
        IDocumentBuilder() {}
        ~IDocumentBuilder() {}

    public:
        virtual void Serialize(const std::wstring &str, std::shared_ptr<IDocument> doc) = 0;
        virtual std::wstring Deserialize(const IDocument *doc) = 0;
    };
}
