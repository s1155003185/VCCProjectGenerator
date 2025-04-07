#pragma once

#include <string>

#include "exception_type.hpp"

namespace vcc
{
    class IResult
    {
        protected:
            IResult() {}
            virtual ~IResult() {}

        public:
            virtual ExceptionType GetExceptionType() const = 0;
            virtual void SetExceptionType(const ExceptionType &exceptionType) const;

            virtual std::wstring GetErrorMessage() const = 0;
            virtual void SetErrorMessage(const std::wstring &errorMessage) const; 
    };
}
