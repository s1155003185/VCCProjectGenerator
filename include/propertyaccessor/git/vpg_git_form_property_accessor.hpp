#pragma once

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGGitFormPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    public:
        VPGGitFormPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGGitFormPropertyAccessor() {}
};
