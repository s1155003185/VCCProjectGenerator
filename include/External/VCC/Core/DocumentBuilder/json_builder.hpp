#pragma once

#include "base_document_builder.hpp"
#include "json.hpp"

namespace vcc
{
    class JsonBuilder : public BaseDocumentBuilder<JsonObject>
    {
        protected:
            std::wstring GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);
            void ParseJsonObject(const std::wstring &str, std::shared_ptr<JsonObject> doc);
            
        public:
            JsonBuilder() = default;
            virtual ~JsonBuilder() {}

            virtual std::wstring Serialize(const JsonObject *doc) override;
            virtual void Deserialize(const std::wstring &str, std::shared_ptr<JsonObject> doc) override;
    };
}
