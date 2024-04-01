#pragma once

#include <memory>
#include <string>

#include "base_manager.hpp"
#include "base_object.hpp"
#include "base_json_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "log_property.hpp"

#include "vpg_project_type.hpp"

using namespace vcc;

const std::wstring MakeFileName = L"Makefile";

class VPGGenerationOption : public BaseObject<VPGGenerationOption>, public BaseJsonObject
{
    // Generation Use
    GETSET(VPGProjectType, ProjectType, VPGProjectType::NA);
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceDestination, L"");

    // --------------------------------------------------
    // Config
    // --------------------------------------------------
    GETSET(std::wstring, Version, L"0.0.1");
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

        virtual std::wstring SerializeJson(const IDocumentBuilder *builder) override;
        virtual void DeserializeJson(std::shared_ptr<IDocument> document) override;
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
