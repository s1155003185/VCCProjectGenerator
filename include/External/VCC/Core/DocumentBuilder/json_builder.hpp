#pragma once

#include "base_document_builder.hpp"
#include "json.hpp"

namespace vcc
{
    class IDocument;
    class JsonBuilder : public BaseDocumentBuilder
    {
        protected:
            std::wstring GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg) const;
            void ParseJsonObject(const std::wstring &str, size_t &pos, std::shared_ptr<Json> doc) const;
            
        public:
            JsonBuilder() = default;
            virtual ~JsonBuilder() {}

            virtual std::wstring Serialize(const IDocument *doc) const  override;
            virtual void Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const override;
            virtual void Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const override;
    };
}
