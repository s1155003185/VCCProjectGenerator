#pragma once

#include <string>
#include <vector>

#include "class_macro.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

class VPGProcessManager
{
    // project
    GETSET(std::wstring, Workspace, L"");
    GETSET(VPGProjectType, VPGProjectType, VPGProjectType::VccComplex);
    GETSET(std::wstring, TagVersion, L"");
    GETSET(std::wstring, SourceDirectory, L"");
    VECTOR(std::wstring, Plugins);
    
    // option
    GETSET(bool, IsSourceFullHistory, false);
    GETSET(bool, IsExcludeUnitTest, false);
    GETSET(bool, IsExcludeExternalUnitTest, false);

    public:
        VPGProcessManager() = default;
        ~VPGProcessManager() {};

        void Add();
        void Update();
        void Generate();
        void Execute(const std::vector<std::wstring> &cmds);
};