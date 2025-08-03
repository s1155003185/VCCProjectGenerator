#pragma once
#include "i_document_builder.hpp"

#include "exception_macro.hpp"

namespace vcc
{
    class BaseDocumentBuilder : public IDocumentBuilder
    {
        protected:
            // Error Message
            GETSET(int64_t, NumberOfCharactersBeforePosForErrorMessage, 10);
            GETSET(int64_t, NumberOfCharactersAfterPosForErrorMessage, 50);
            std::wstring getErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;

        protected:
            BaseDocumentBuilder() = default;
            virtual ~BaseDocumentBuilder() {}
    };
}