#pragma once

#include <memory>
#include <string>

#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

class VPGVccGenerationManager : public VPGBaseGenerationManager
{
    private:
        std::wstring adjustAppliationCpp(const std::wstring &fileContent) const;
        
    public:
        VPGVccGenerationManager() : VPGVccGenerationManager(nullptr, L"", nullptr) {}
        VPGVccGenerationManager(std::shared_ptr<vcc::LogConfig> logConfig, std::wstring workspace, std::shared_ptr<VPGConfig> option) : VPGBaseGenerationManager(logConfig, workspace, option) {}
        virtual ~VPGVccGenerationManager() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGVccGenerationManager>(*this);
        }

        std::vector<std::wstring> getUpdateList() const;
        std::vector<std::wstring> getUpdateUnitTestList() const;

        void CreateVccJson(bool isNew) const;
        void ReadVccJson() const;

        virtual void add() const override;
        virtual void update() const override;
        virtual void generate() const override;
};
