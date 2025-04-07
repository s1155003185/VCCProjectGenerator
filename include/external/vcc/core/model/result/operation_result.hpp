#pragma once

#include <memory>

#include "base_result.hpp"
#include "object_type.hpp"

namespace vcc
{
    class OperationResult : public BaseResult
    {
        public:
            OperationResult() : BaseResult(ObjectType::OperationResult) {}
            virtual ~OperationResult() {}
            
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<OperationResult>();
            }
    };
}
