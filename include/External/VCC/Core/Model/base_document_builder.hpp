#include "i_document_builder.hpp"

namespace vcc
{
    class BaseDocumentBuilder : public IDocumentBuilder
    {
        protected:
            BaseDocumentBuilder() = default;
            ~BaseDocumentBuilder() {}
    };
}