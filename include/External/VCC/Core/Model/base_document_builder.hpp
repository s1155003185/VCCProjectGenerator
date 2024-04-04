#include "i_document_builder.hpp"

#include "exception_macro.hpp"

namespace vcc
{
    class BaseDocumentBuilder : public IDocumentBuilder
    {
        protected:
            BaseDocumentBuilder() = default;
            ~BaseDocumentBuilder() {}
    };
}