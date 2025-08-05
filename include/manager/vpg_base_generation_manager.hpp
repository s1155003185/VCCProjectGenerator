#pragma once

#include <memory>

#include "base_manager.hpp"
#include "exception_macro.hpp"
#include "i_vpg_generation_manager.hpp"
#include "vpg_config.hpp"

const std::wstring MakeFileName = L"Makefile";
const std::wstring unittestFolderName = L"unittest";

class VPGBaseGenerationManager : public vcc::BaseManager, public IVPGGenerationManager
{
    GETSET(std::wstring, Workspace, L"");
    GETSET_SPTR_NULL(VPGConfig, Option);
    
    private:
        VPGBaseGenerationManager() = delete;
    
    protected:
        void validateOption() const;
    public:
        VPGBaseGenerationManager(std::shared_ptr<vcc::LogConfig> logConfig, std::wstring &workspace, std::shared_ptr<VPGConfig> option) : BaseManager(logConfig)
        { 
            this->_Workspace = workspace;
            assert(option != nullptr);
            this->_Option = option;
        }

        virtual ~VPGBaseGenerationManager() {}

        void getDLLTestFileContent(std::wstring &fileContent) const;
        
        void CreateWorkspaceDirectory() const;
        void CreateBasicProject() const;
        std::wstring AdjustMakefile(const std::wstring &fileContent) const;
        std::wstring AdjustVSCodeLaunchJson(const std::wstring &fileContent) const;

        void syncWorkspace(const vcc::LogConfig *logConfig, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters) const;
        
        virtual void add() const override = 0;

        virtual void update() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
        }

        virtual void generate() const override
        {
            THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
        }
};
