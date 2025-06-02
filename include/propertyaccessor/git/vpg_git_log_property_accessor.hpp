#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

class VPGGitLogPropertyAccessor : public vcc::BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGGitLogPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~VPGGitLogPropertyAccessor() {}
};
