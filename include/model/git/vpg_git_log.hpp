#pragma once

#include "base_object.hpp"
#include "class_macro.hpp"
#include "git_service.hpp"
#include "object_type.hpp"

class VPGGitLog : public vcc::GitLog
{

    public:
        VPGGitLog() : vcc::GitLog()
        {
            _ObjectType = ObjectType::GitLog;
        }
        virtual ~VPGGitLog() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGGitLog>(*this);
        }
};
