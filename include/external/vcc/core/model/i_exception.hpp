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
            virtual ExceptionType GetErrorType() const = 0;
            virtual int64_t GetErrorCode() const = 0;
            virtual std::wstring GetErrorMessage() const = 0;
            virtual void SetErrorMessage(const std::wstring &message) const = 0;
    };
}
