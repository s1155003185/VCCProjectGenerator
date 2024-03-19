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
        virtual void Serialize(const std::wstring &str, std::shared_ptr<Document> doc) = 0;
        virtual std::wstring Deserialize(const Document *doc) = 0;
    };
}
