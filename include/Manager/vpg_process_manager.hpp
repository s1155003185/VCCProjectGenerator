#pragma once

#include <string>
#include <memory>
#include <vector>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "i_vpg_generation_manager.hpp"
#include "log_property.hpp"
#include "vpg_base_generation_manager.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGProcessManager : public BaseManager<VPGProcessManager>
{
    // project
    GETSET_SPTR(VPGGenerationOption, Option);

    private:
        std::shared_ptr<IVPGGenerationManager> GetGenerationManager();

    public:
        VPGProcessManager(std::shared_ptr<LogProperty> logProperty) : BaseManager(logProperty) {};
        ~VPGProcessManager() {};

        void InitLogProperty();
        // Ensure VPG Generator have same version as Versioning Commond Codebase Response
        void VerifyLocalResponse();

        bool IsUpdateAvaliable();
        
        void Add();
        void Update();
        void Generate();
        void Execute(const std::vector<std::wstring> &cmds);
};