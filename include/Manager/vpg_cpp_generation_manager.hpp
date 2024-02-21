#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGCppGenerationManager : public VPGBaseGenerationManager
{
    public:
        VPGCppGenerationManager(std::shared_ptr<LogProperty> logProperty, VPGProjectType projectType) : VPGBaseGenerationManager(logProperty, projectType) {}
        virtual ~VPGCppGenerationManager() {}

        virtual void Add(const std::wstring &srcWorkspace, const std::wstring &destWorkspace, const VPGGenerationOption *option) override;
};
