#pragma once

#include <string>
#include <vector>

#include "class_macro.hpp"
#include "log_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGProcessManager
{
    GETUPTR(LogProperty, LogProperty, LogPropertyType::None);

    // project
    GETSET(std::wstring, Workspace, L"");
    GETSET(VPGProjectType, VPGProjectType, VPGProjectType::NA);
    GETSET(std::wstring, Branch, L"");
    VECTOR(std::wstring, Plugins);
    
    // option
    GETSET(bool, IsExcludeUnitTest, false);
    GETSET(bool, IsExcludeExternalUnitTest, false);

    public:
        VPGProcessManager() = default;
        ~VPGProcessManager() {};

        void InitLogProperty();
        void VerifyLocalResponse();

        void Add();
        void Update();
        void Generate();
        void Execute(const std::vector<std::wstring> &cmds);
};