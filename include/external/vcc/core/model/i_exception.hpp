#pragma once
#include <cstdint>
#include <exception>
#include <string>

#include "exception_type.hpp"

namespace vcc
{
    class IException : public std::exception
    {
        protected:
            IException() {}
            virtual ~IException() {}
        public:
            virtual ExceptionType getErrorType() const = 0;
            virtual int64_t getErrorCode() const = 0;
            virtual std::wstring getErrorMessage() const = 0;
            virtual void setErrorMessage(const std::wstring &message) const = 0;
    };
}
