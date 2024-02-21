#pragma once

#include <memory>
#include <string>

#include "base_manager.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"

#include "vpg_project_type.hpp"

using namespace vcc;

const std::wstring MakeFileName = L"Makefile";

class VPGGenerationOption : public BaseObject<VPGGenerationOption>
{
    GETSET(std::wstring, ProjectName, L"");
    GETSET(std::wstring, DllProjectName, L"");
    GETSET(std::wstring, ExeProjectName, L"");
    GETSET(std::wstring, GtestProjName, L"");

    VECTOR(std::wstring, Plugins);

    public:;
        VPGGenerationOption() = default;
        virtual ~VPGGenerationOption() {}
};

class VPGBaseGenerationManager : public BaseManager
{
    GETSET(VPGProjectType, ProjectType, VPGProjectType::NA);
    GET_SPTR(LogProperty, LogProperty);
    
    public:
        VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, VPGProjectType projectType);
        virtual ~VPGBaseGenerationManager() {}

        virtual std::wstring AdjustMakefile(const std::wstring &fileContent, const VPGGenerationOption *option);

        virtual void Add(const std::wstring &srcWorkspace, const std::wstring &destWorkspace, const VPGGenerationOption *option);
        virtual void Update(const std::wstring &srcWorkspace, const std::wstring &destWorkspace, const VPGGenerationOption *option);
        virtual void Generate(const std::wstring &srcWorkspace, const std::wstring &destWorkspace, const VPGGenerationOption *option);
};
