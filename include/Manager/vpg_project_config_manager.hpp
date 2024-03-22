#pragma once

#include <memory>

#include "base_manager.hpp"

using namespace vcc;

class VPGGenerationOption;
class VPGProjectConfigManager : public BaseManager<VPGProjectConfigManager>
{
    public:
        VPGProjectConfigManager() = default;
        virtual ~VPGProjectConfigManager() {}

        void serialize(const VPGGenerationOption *option);
        void unserialize(VPGGenerationOption *option);
};
