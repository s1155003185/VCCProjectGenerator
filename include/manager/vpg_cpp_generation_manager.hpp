#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGCppGenerationManager : public VPGBaseGenerationManager
{
    public:
        VPGCppGenerationManager(std::shared_ptr<LogConfig> logConfig, std::wstring workspace, std::shared_ptr<VPGGenerationOption> option) : VPGBaseGenerationManager(logConfig, workspace, option) {}
        virtual ~VPGCppGenerationManager() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGCppGenerationManager>(*this);
        }

        virtual void Add() const override;
};
