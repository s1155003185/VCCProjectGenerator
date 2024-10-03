#pragma once
#include "i_document.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"

#include "object_type.hpp"

namespace vcc
{
    template <typename Derived>
    class BaseDocument : public IDocument, public BaseObject<Derived>
    {
        protected:
            BaseDocument() {}
            virtual ~BaseDocument() {}
    };
}
