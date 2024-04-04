#pragma once

#include <string>
#include <memory>
#include <vector>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGProcessManager : public BaseManager<VPGProcessManager>
{
    // project
    GETSET(std::wstring, Workspace, L"");
    GETSET(VPGProjectType, VPGProjectType, VPGProjectType::NA);
    GETSET(std::wstring, Tag, L"");
    GETSET(int64_t, LogDepth, -1);
    VECTOR(std::wstring, Plugins);
    
    // option
    GETSET(bool, IsExcludeUnitTest, false);
    GETSET(bool, IsExcludeExternalUnitTest, false);

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