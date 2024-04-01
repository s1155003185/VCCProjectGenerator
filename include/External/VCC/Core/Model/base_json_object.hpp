#pragma once

#include "i_json_object.hpp"

namespace vcc
{
    class BaseJsonObject : public IJsonObject
    {
    protected:
        BaseJsonObject() = default;
        virtual ~BaseJsonObject() {}
    };
}
