#include "vpg_base_generation_manager.hpp"

#include <assert.h>
#include <memory>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

VPGBaseGenerationManager::VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, VPGProjectType projectType)
{ 
    this->_LogProperty = logProperty;
    this->_ProjectType = projectType;
}

std::wstring VPGBaseGenerationManager::AdjustMakefile(const std::wstring &fileContent, const VPGGenerationOption *option)
{
    std::wstring result = L"";
    TRY_CATCH(
        DECLARE_SPTR(XMLElement, elements);
        VPGCodeReader reader(L"#");
        reader.Parse(fileContent, elements);
        for (std::shared_ptr<vcc::XMLElement> element : elements->GetChildren()) {
            if (element->GetNamespace() == L"vcc" && element->GetName() == L"name") {
                std::wstring projName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
                std::wstring dllName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
                std::wstring exeName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
                std::wstring gtestName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
        
                result += L"# <vcc:name sync=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJECT_NAME :=" + projName + L"\r\n";
                result += L"DLL_PROJ_NAME :=" + dllName + L"\r\n";
                result += L"EXE_PROJ_NAME :=" + exeName + L"\r\n";
                result += L"GTEST_PROJ_NAME :=" + gtestName + L"\r\n";
                result += L"# </vcc:name>";
            } else
                result += element->GetFullText();
        }
    )
    return result;
}

void VPGBaseGenerationManager::Add(const std::wstring &/* srcWorkspace */, const std::wstring &/* destWorkspace */, const VPGGenerationOption */* option */)
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Update(const std::wstring &/* srcWorkspace */, const std::wstring &/* destWorkspace */, const VPGGenerationOption */* option */)
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Generate(const std::wstring &/* srcWorkspace */, const std::wstring &/* destWorkspace */, const VPGGenerationOption */* option */)
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
}