#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGVccGenerationManager : public VPGBaseGenerationManager<VPGVccGenerationManager>
{
    public:
        VPGVccGenerationManager() : VPGVccGenerationManager(nullptr, L"", nullptr) {}
        VPGVccGenerationManager(std::shared_ptr<LogConfig> logConfig, std::wstring workspace, std::shared_ptr<VPGGenerationOption> option) : VPGBaseGenerationManager(logConfig, workspace, option) {}
        virtual ~VPGVccGenerationManager() {}

        std::vector<std::wstring> GetUpdateList() const;
        std::vector<std::wstring> GetUpdateUnitTestList() const;

        void CreateVccJson() const;
        void ReadVccJson() const;

        virtual void Add() const override;
        virtual void Update() const override;
        virtual void Generate() const override;
};
