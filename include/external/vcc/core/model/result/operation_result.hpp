#pragma once

#include <memory>
#include <string>

#include "base_result.hpp"
#include "exception_type.hpp"
#include "object_type.hpp"

namespace vcc
{
    class OperationResult : public BaseResult
    {
        public:
            OperationResult() : OperationResult(ExceptionType::NoError, L"") {}
            OperationResult(const ExceptionType &exceptionType, const std::wstring &message) : BaseResult(ObjectType::OperationResult, exceptionType, message) {}
            virtual ~OperationResult() {}
            
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<OperationResult>();
            }
    };
}
