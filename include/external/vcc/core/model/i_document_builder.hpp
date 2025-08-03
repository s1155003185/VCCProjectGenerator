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
        virtual ~IDocumentBuilder() {}

    public:
        virtual std::wstring serialize(const IDocument *doc) const = 0;
        virtual void deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const = 0;
        virtual void deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const = 0;
    };
}
