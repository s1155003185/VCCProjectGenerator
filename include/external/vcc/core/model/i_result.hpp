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
            virtual void SetExceptionType(const ExceptionType &exceptionType) const = 0;

            virtual std::wstring GetMessage() const = 0;
            virtual void SetMessage(const std::wstring &errorMessage) const = 0;

            virtual bool IsError() const = 0;
            virtual bool IsWarning() const = 0;
            virtual bool IsThrowException() const = 0;
    };
}
