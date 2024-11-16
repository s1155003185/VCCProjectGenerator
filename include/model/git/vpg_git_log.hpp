#pragma once

#include "base_object.hpp"
#include "class_macro.hpp"
#include "git_service.hpp"
#include "object_type.hpp"

using namespace vcc;

class VPGGitLog : public GitLog
{

    public:
        VPGGitLog() : GitLog()
        {
            _ObjectType = ObjectType::GitLog;
        }
        virtual ~VPGGitLog() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGGitLog>(*this);
        }
};
