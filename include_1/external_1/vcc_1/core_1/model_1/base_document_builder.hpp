#pragma once
#include "i_document_builder.hpp"

#include "exception_macro.hpp"

namespace vcc
{
    class BaseDocumentBuilder : public IDocumentBuilder
    {
        protected:
            // Error Message
            GETSET(size_t, NumberOfCharactersBeforePosForErrorMessage, 10);
            GETSET(size_t, NumberOfCharactersAfterPosForErrorMessage, 50);
            std::wstring GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;

        protected:
            BaseDocumentBuilder() = default;
            virtual ~BaseDocumentBuilder() {}
    };
}