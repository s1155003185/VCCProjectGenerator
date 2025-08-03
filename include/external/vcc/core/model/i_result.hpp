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
            virtual ExceptionType getExceptionType() const = 0;
            virtual void setExceptionType(const ExceptionType &exceptionType) const = 0;

            virtual std::wstring getMessage() const = 0;
            virtual void setMessage(const std::wstring &errorMessage) const = 0;

            virtual bool isError() const = 0;
            virtual bool isWarning() const = 0;
            virtual bool isThrowException() const = 0;
    };
}
