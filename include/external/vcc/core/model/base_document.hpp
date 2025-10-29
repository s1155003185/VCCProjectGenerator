#pragma once
#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "object_type.hpp"

namespace vcc {
class BaseDocument : public IDocument, public BaseObject {
   protected:
    BaseDocument() {}
    virtual ~BaseDocument() {}
};
}  // namespace vcc
