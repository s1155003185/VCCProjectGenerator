#pragma once

#include <string>

#include "base_property_accessor.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;

class VPGGitLogPropertyAccessor : public BasePropertyAccessor
{
    PROPERTY_ACCESSOR_HEADER(bool, Bool)
    PROPERTY_ACCESSOR_HEADER(long, Long)
    PROPERTY_ACCESSOR_HEADER(std::wstring, String)

    PROPERTY_ACCESSOR_CONTAINER_HEADER

    public:
        VPGGitLogPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~VPGGitLogPropertyAccessor() {}
};
