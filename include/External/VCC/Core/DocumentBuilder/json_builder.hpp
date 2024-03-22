#pragma once

#include "base_document_builder.hpp"
#include "json.hpp"

namespace vcc
{
    class JsonBuilder : public BaseDocumentBuilder<Json>
    {
        public:
            JsonBuilder() = default;
            virtual ~JsonBuilder() {}

            virtual std::wstring Serialize(const Json *doc) override;
            virtual void Deserialize(const std::wstring &str, std::shared_ptr<Json> doc) override;
    };
}
