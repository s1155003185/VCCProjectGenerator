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

            virtual void Serialize(const std::wstring &str, std::shared_ptr<Json> doc) override;
            virtual std::wstring Deserialize(const Json *doc) override;
    };
}
