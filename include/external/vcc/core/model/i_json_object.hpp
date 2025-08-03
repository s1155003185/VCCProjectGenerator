#pragma once

#include <memory>
#include <string>

namespace vcc
{
    class IDocument;
    class IDocumentBuilder;
    class Json;
    class IJsonObject
    {
        public:
            IJsonObject() {}
            virtual ~IJsonObject() {}

            virtual std::shared_ptr<Json> ToJson() const = 0;
            virtual std::wstring serializeJson(const IDocumentBuilder *builder) const = 0;
            virtual void deserializeJson(std::shared_ptr<IDocument> document) = 0;
            virtual void deserializeJsonString(const IDocumentBuilder *builder, const std::wstring &jsonStr) = 0;
    };
}
