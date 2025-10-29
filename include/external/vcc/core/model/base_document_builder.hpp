#pragma once
#include "exception_macro.hpp"
#include "i_document_builder.hpp"

namespace vcc {
class BaseDocumentBuilder : public IDocumentBuilder {
   protected:
    // Error Message
    GETSET(int64_t, NumberOfCharactersBeforePosForErrorMessage, 10);
    GETSET(int64_t, NumberOfCharactersAfterPosForErrorMessage, 50);
    std::wstring getErrorMessage(const std::wstring& str, const size_t& pos,
                                 const std::wstring& msg) const;

   protected:
    BaseDocumentBuilder() = default;
    virtual ~BaseDocumentBuilder() {}
};
}  // namespace vcc