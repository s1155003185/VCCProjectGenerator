#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGCppGenerationManager : public VPGBaseGenerationManager<VPGCppGenerationManager>
{
    public:
        VPGCppGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option) : VPGBaseGenerationManager(logProperty, option) {}
        virtual ~VPGCppGenerationManager() {}

        virtual void Add() const override;
};
