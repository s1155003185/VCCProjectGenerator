#include "i_document_builder.hpp"

namespace vcc
{    
    template<typename Document>
    class BaseDocumentBuilder : public IDocumentBuilder<Document>
    {
        protected:
            BaseDocumentBuilder() = default;
            ~BaseDocumentBuilder() {}
    };
}