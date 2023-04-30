#pragma once
#include <exception>
#include <string>

#include "exception_type.hpp"

namespace vcc
{
    class IException : public std::exception
    {
        protected:
            IException() {}
            ~IException() {}
        public:
            virtual ExceptionType GetErrorType() = 0;
            virtual int64_t GetErrorCode() = 0;
            virtual std::wstring GetErrorMessage() = 0;
            virtual void SetErrorMessage(std::wstring message) = 0;
    };
}
