#pragma once

#include <memory>
#include <string>

#include "base_document_builder.hpp"

namespace vcc
{
    class IDocument;
    class ConfigBuilder : public BaseDocumentBuilder
    {
        public:
            ConfigBuilder() = default;
            virtual ~ConfigBuilder() {}

            virtual std::wstring serialize(const IDocument *doc) const override;
            virtual void deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const override;
            virtual void deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const override;
    };
}