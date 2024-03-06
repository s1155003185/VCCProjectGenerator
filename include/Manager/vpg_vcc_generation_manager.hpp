#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGVccGenerationManager : public VPGBaseGenerationManager
{
    public:
        VPGVccGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option) : VPGBaseGenerationManager(logProperty, option) {}
        virtual ~VPGVccGenerationManager() {}

        std::vector<std::wstring> GetUpdateList();
        std::vector<std::wstring> GetUpdateUnitTestList();

        virtual void Add() override;
        virtual void Update() override;
        virtual void Generate() override;
};
