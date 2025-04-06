#pragma once

#include "base_object.hpp"
#include "class_macro.hpp"
#include "exception_type.hpp"
#include "i_result.hpp"

namespace vcc
{
    class BaseResult : public IResult, public BaseObject
    {
        protected:
            mutable ExceptionType _ExceptionType = ExceptionType::NoError;
            mutable std::wstring _ErrorMessage = L"";

            BaseResult(const ObjectType &type) : BaseObject(type) {}
            virtual ~BaseResult() {}

        public:
            virtual ExceptionType GetExceptionType() const override
            {
                return _ExceptionType;
            }

            virtual void SetExceptionType(const ExceptionType &exceptionType) const override
            {
                _ExceptionType = exceptionType;
            }

            virtual std::wstring GetErrorMessage() const override
            {
                return _ErrorMessage;
            }

            virtual void SetErrorMessage(const std::wstring &errorMessage) const override
            {
                _ErrorMessage = errorMessage;
            }
    };
}
