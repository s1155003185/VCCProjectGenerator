#pragma once

#include "base_document_builder.hpp"
#include "json.hpp"

namespace vcc
{
    class IDocument;
    class JsonBuilder : public BaseDocumentBuilder
    {
        // For Serialize Only
        GETSET(bool, IsBeautify, false);
        GETSET(size_t, Level, 0);
        GETSET(std::wstring, Indent, INDENT);
        GETSET(std::wstring, NewLineCharacter, L"\r\n");
        GETSET(std::wstring, NameColonSpace, L"");
        GETSET(std::wstring, ColonValueSpace, L" ");

        // Error Message
        GETSET(size_t, NumberOfCharactersBeforePosForErrorMessage, 10);
        GETSET(size_t, NumberOfCharactersAfterPosForErrorMessage, 50);

        private:
            std::wstring GetCurrentIndent() const;

        protected:
            std::wstring GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;
            void ParseJsonObject(const std::wstring &str, size_t &pos, std::shared_ptr<Json> doc) const;
            
        public:
            JsonBuilder() = default;
            virtual ~JsonBuilder() {}

            virtual std::wstring Serialize(const IDocument *doc) const  override;
            virtual void Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const override;
            virtual void Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const override;
    };
}
