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
            mutable std::wstring _Message = L"";

            BaseResult(const ObjectType &type) : BaseObject(type) {}
            BaseResult(const ObjectType &type, const ExceptionType &exceptionType, const std::wstring &message) : BaseResult(type)
            {
                _ExceptionType = exceptionType;
                _Message = message;
            }
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

            virtual std::wstring GetMessage() const override
            {
                return _Message;
            }

            virtual void SetMessage(const std::wstring &message) const override
            {
                _Message = message;
            }
            
            virtual bool IsError() const override
            {
                return _ExceptionType != ExceptionType::NoError && _ExceptionType != ExceptionType::Warning;
            }

            virtual bool IsWarning() const override
            {
                return _ExceptionType == ExceptionType::Warning;
            }

            virtual bool IsThrowException() const override
            {
                return false;
            }
    };
}
