#include "vpg_base_generation_manager.hpp"

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

VPGBaseGenerationManager::VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option)
{ 
    assert(logProperty != nullptr);
    assert(option != nullptr);
    this->_LogProperty = logProperty;
    this->_Option = option;
}

void VPGBaseGenerationManager::ValidateOption()
{
    assert(!IsBlank(_Option->GetWorkspaceSource()));
    assert(!IsBlank(_Option->GetWorkspaceDestination()));
    assert(!IsBlank(_Option->GetProjectName()));
    assert(!IsBlank(_Option->GetProjectNameDLL()) || !IsBlank(_Option->GetProjectNameEXE()));
}

void VPGBaseGenerationManager::CreateWorkspaceDirectory()
{
    ValidateOption();
    TRY_CATCH(
        // All type has same project structure
        std::vector<std::wstring> checkList;
        // bin
        checkList.push_back(L"bin");
        checkList.push_back(L"bin/Debug");
        checkList.push_back(L"bin/Release");

        // include
        checkList.push_back(L"include");

        // lib
        checkList.push_back(L"lib");

        // src
        checkList.push_back(L"src");

        // unittest
        if (!IsBlank(_Option->GetProjectNameGtest()))
            checkList.push_back(_Option->GetProjectNameGtest());
        
        std::wstring workspace = _Option->GetWorkspaceDestination();
        for (auto path : checkList) {
            std::wstring absPath = ConcatPath(workspace, path);
            if (!IsDirectoryExists(absPath)) {
                CreateDirectory(absPath);
                LogService::LogInfo(this->GetLogProperty().get(), L"", L"Create Directory: " + path);
            }        
        }
    )
}

void VPGBaseGenerationManager::CreateBasicProject()
{
    ValidateOption();
    TRY_CATCH(
        this->CreateWorkspaceDirectory();

        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        CopyFile(ConcatPath(src, MakeFileName), ConcatPath(dest, MakeFileName));
        if (!IsBlank(_Option->GetProjectNameEXE())) {
            CopyFile(ConcatPath(src, L"main.cpp"), ConcatPath(dest, L"main.cpp"));
        }
        if (!IsBlank(_Option->GetProjectNameDLL())) {
            CopyFile(ConcatPath(src, L"DllEntryPoint.cpp"), ConcatPath(dest, L"DllEntryPoint.cpp"));
            CopyFile(ConcatPath(src, L"DllFunctions.cpp"), ConcatPath(dest, L"DllFunctions.cpp"));
            CopyFile(ConcatPath(src, L"DllFunctions.h"), ConcatPath(dest, L"DllFunctions.h"));
        }
        if (!IsBlank(_Option->GetProjectNameGtest())) {
            CopyFile(ConcatPath(ConcatPath(src, L"unittest"), L"gtest_main.cpp"), ConcatPath(ConcatPath(dest, _Option->GetProjectNameGtest()), L"gtest_main.cpp"), true);
        }
        // Cannot Copy
        AppendFileOneLine(ConcatPath(dest, L"LICENSE"), L"", true);
        AppendFileOneLine(ConcatPath(dest, L"README.md"), L"", true);

        if (IsFileExists(ConcatPath(src, MakeFileName))) {
            // Makefile
            std::wstring makefilePath = ConcatPath(dest, MakeFileName);
            std::wstring makefileContent = ReadFile(makefilePath);
            WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        }
    )
}

std::wstring VPGBaseGenerationManager::AdjustMakefile(const std::wstring &fileContent)
{
    ValidateOption();
    std::wstring result = L"";
    TRY_CATCH(
        DECLARE_SPTR(XMLElement, elements);
        VPGCodeReader reader(L"#");
        reader.Parse(fileContent, elements);
        for (std::shared_ptr<vcc::XMLElement> element : elements->GetChildren()) {
            if (element->GetNamespace() == L"vcc" && element->GetName() == L"name") {
                std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
                std::wstring dllName = !IsBlank(_Option->GetProjectNameDLL()) ? (L" " + _Option->GetProjectNameDLL()) : L"";
                std::wstring exeName = !IsBlank(_Option->GetProjectNameEXE()) ? (L" " + _Option->GetProjectNameEXE()) : L"";
                std::wstring gtestName = !IsBlank(_Option->GetProjectNameGtest()) ? (L" " + _Option->GetProjectNameGtest()) : L"";
        
                result += L"# <vcc:name sync=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJ_NAME :=" + projName + L"\r\n";
                result += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
                result += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
                result += L"PROJ_NAME_GTEST :=" + gtestName + L"\r\n";
                result += L"# </vcc:name>";
            } else
                result += element->GetFullText();
        }
    )
    return result;
}

void VPGBaseGenerationManager::Add()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Update()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Generate()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
}