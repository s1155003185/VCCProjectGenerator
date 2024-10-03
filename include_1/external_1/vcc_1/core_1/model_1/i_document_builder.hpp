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
        virtual std::wstring Serialize(const IDocument *doc) const = 0;
        virtual void Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const = 0;
        virtual void Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const = 0;
    };
}
