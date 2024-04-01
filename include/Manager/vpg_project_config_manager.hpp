#pragma once

#include <memory>
#include <string>

#include "base_manager.hpp"

using namespace vcc;

class VPGGenerationOption;
class VPGProjectConfigManager : public BaseManager<VPGProjectConfigManager>
{
    public:
        VPGProjectConfigManager() = default;
        virtual ~VPGProjectConfigManager() {}

        void Serialize(const std::wstring workspace, const VPGGenerationOption *option);
        void Deserialize(const std::wstring workspace, shared_ptr<VPGGenerationOption> option);
};
