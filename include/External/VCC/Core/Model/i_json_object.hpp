#pragma once

#include <memory>
#include <string>

#include "json.hpp"

namespace vcc
{
    class IDocument;
    class IDocumentBuilder;
    class IJsonObject
    {
        public:
            IJsonObject() {}
            virtual ~IJsonObject() {}

            virtual std::wstring SerializeJson(const IDocumentBuilder *builder) = 0;
            virtual void DeserializeJson(std::shared_ptr<IDocument> document) = 0;
    };
}
