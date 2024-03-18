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
    GETSET(VPGProjectType, ProjectType, VPGProjectType::NA);
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceDestination, L"");

    GETSET(bool, IsGit, false);
    
    // Project
    GETSET(std::wstring, ProjectPrefix, L"");

    GETSET(std::wstring, ProjectName, L"");
    GETSET(std::wstring, ProjectNameDLL, L"");
    GETSET(std::wstring, ProjectNameEXE, L"");
    GETSET(std::wstring, ProjectNameGtest, L"");

    GETSET(bool, IsExcludeVCCUnitTest, false);

    VECTOR(std::wstring, Plugins);

    // Files    
    GETSET(std::wstring, ActionTypeDirectory, L"include/Type");
    GETSET(std::wstring, ExceptionTypeDirectory, L"include/Type");
    GETSET(std::wstring, ManagerTypeDirectory, L"include/Type");
    GETSET(std::wstring, ObjectTypeDirectory, L"include/Type");

    GETSET(std::wstring, TypeWorkspace, L"include/Type");
    GETSET(std::wstring, ObjectHppDirectory, L"include/Module");
    GETSET(std::wstring, ObjectTypeHppDirectory, L"include/Type");
    GETSET(std::wstring, PropertyAccessorHppDirectory, L"include/PropertyAccessor");
    GETSET(std::wstring, PropertyAccessorCppDirectory, L"src/PropertyAccessor");

    public:;
        VPGGenerationOption() = default;
        virtual ~VPGGenerationOption() {}
};

class VPGBaseGenerationManager : public BaseManager<VPGBaseGenerationManager>
{
    GET_SPTR(LogProperty, LogProperty);
    GET_SPTR(VPGGenerationOption, Option);
    
    protected:
        virtual void ValidateOption();
    public:
        VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option);
        virtual ~VPGBaseGenerationManager() {}

        virtual std::wstring GetDLLTestFileContent();
        
        virtual void CreateWorkspaceDirectory();
        virtual void CreateBasicProject();
        virtual std::wstring AdjustMakefile(const std::wstring &fileContent);

        void SyncWorkspace(const LogProperty *logProperty, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters);
            
        virtual void Add();
        virtual void Update();
        virtual void Generate();
};
