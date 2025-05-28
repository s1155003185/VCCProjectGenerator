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
            virtual std::wstring SerializeJson(const IDocumentBuilder *builder) const = 0;
            virtual void DeserializeJson(std::shared_ptr<IDocument> document) = 0;
            virtual void DeserializeJsonString(const IDocumentBuilder *builder, const std::wstring &jsonStr) = 0;
    };
}
