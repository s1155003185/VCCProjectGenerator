#pragma once

#include <string>
#include <memory>
#include <vector>

#include "class_macro.hpp"
#include "log_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGProcessManager
{
    GETSPTR(LogProperty, LogProperty, LogPropertyType::None);

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
        VPGProcessManager() = default;
        ~VPGProcessManager() {};

        void InitLogProperty();
        // Ensure VPG Generator have same version as Versioning Commond Codebase Response
        void VerifyLocalResponse();

        void Add();
        void Update();
        void Generate();
        void Execute(const std::vector<std::wstring> &cmds);
};