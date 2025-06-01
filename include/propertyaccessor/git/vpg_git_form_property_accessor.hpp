#pragma once

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

class VPGGitFormPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<vcc::IObject>, Object)

    public:
        VPGGitFormPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGGitFormPropertyAccessor() {}
};
