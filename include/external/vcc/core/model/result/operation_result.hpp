#pragma once

#include "base_result.hpp"
#include "object_type.hpp"

namespace vcc
{
    class OperationResult : public BaseResult
    {
        public:
            OperationResult() : BaseResult(ObjectType::OperationResult) {}
            virtual ~OperationResult() {}
    };
}
