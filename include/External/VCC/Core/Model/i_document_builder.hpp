#pragma once

#include <memory>
#include <string>

namespace vcc
{
    class IDocument;
    
    template<typename Document>
    class IDocumentBuilder
    {
    protected:
        IDocumentBuilder() {}
        ~IDocumentBuilder() {}

    public:
        virtual std::wstring Serialize(const Document *doc) = 0;
        virtual void Deserialize(const std::wstring &str, std::shared_ptr<Document> doc) = 0;
    };
}
