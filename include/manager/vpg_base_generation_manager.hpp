#pragma once

#include <memory>

#include "base_manager.hpp"
#include "exception_macro.hpp"
#include "i_vpg_generation_manager.hpp"
#include "vpg_config.hpp"

using namespace vcc;

const std::wstring MakeFileName = L"Makefile";
const std::wstring unittestFolderName = L"unittest";

class VPGBaseGenerationManager : public BaseManager, public IVPGGenerationManager
{
    GETSET(std::wstring, Workspace, L"");
    GETSET_SPTR(VPGConfig, Option);
    
    protected:
        void ValidateOption() const;
    public:
        VPGBaseGenerationManager(std::shared_ptr<LogConfig> logConfig, std::wstring &workspace, std::shared_ptr<VPGConfig> option) : BaseManager(logConfig)
        { 
            this->_Workspace = workspace;
            assert(option != nullptr);
            this->_Option = option;
        }

        virtual ~VPGBaseGenerationManager() {}

        void GetDLLTestFileContent(std::wstring &fileContent) const;
        
        void CreateWorkspaceDirectory() const;
        void CreateBasicProject() const;
        std::wstring AdjustMakefile(const std::wstring &fileContent) const;
        std::wstring AdjustVSCodeLaunchJson(const std::wstring &fileContent) const;

        void SyncWorkspace(const LogConfig *logConfig, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters) const;
        
        virtual void Add() const override = 0;

        virtual void Update() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
        }

        virtual void Generate() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
        }
};
