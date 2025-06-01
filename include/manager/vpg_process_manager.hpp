#pragma once

#include <string>
#include <memory>
#include <vector>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "i_vpg_generation_manager.hpp"
#include "log_config.hpp"
#include "vpg_config.hpp"
#include "vpg_project_type.hpp"

class VPGProcessManager : public vcc::BaseManager
{
    // project
    GETSET(std::wstring, Workspace, L"");
    GETSET_SPTR_NULL(VPGConfig, Option);

    private:
        VPGProcessManager() = delete;
        std::shared_ptr<IVPGGenerationManager> GetGenerationManager();

    public:
        VPGProcessManager(std::shared_ptr<vcc::LogConfig> logConfig) : BaseManager(logConfig)
        {
            this->_Option = std::make_shared<VPGConfig>();
        }
        
        ~VPGProcessManager() {};

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<VPGProcessManager>(*this);
        }

        void InitLogConfig();
        // Ensure VPG Generator have same version as Versioning Commond Codebase Response
        void VerifyLocalResponse();

        bool IsUpdateAvaliable();
        
        void Add();
        void Update();
        void Generate();
        void Execute(const std::vector<std::wstring> &cmds);
};