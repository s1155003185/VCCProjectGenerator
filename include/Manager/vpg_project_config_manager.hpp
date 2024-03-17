#pragma once

#include <memonry>

#include "base_manager.hpp"

using namespace vcc;

class VPGGenerationOption;
class VPGProjectConfigManager : public BaseManager
{
    public:
        VPGProjectConfigManager() = default;
        virtual ~VPGProjectConfigManager() {}

        void serialize(const VPGGenerationOption *option);
        void unserialize(VPGGenerationOption *option);
};
