#pragma once
#include <string>

#include "base_exception.hpp"
#include "class_macro.hpp"
#include "exception_type.hpp"
#include "string_helper.hpp"

namespace vcc {
// General Exception
class Exception : public BaseException {
   private:
    Exception() {}

   public:
    Exception(ExceptionType type, std::wstring message) : BaseException(type, message) {}
    ~Exception() {}
};
}  // namespace vcc
