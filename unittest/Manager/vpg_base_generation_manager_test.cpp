#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

#include "vpg_project_type.hpp"
#include "vpg_cpp_generation_manager.hpp"

using namespace std;
using namespace vcc;

class VPGBaseGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    MANAGER(VPGBaseGenerationManager, Manager, _LogProperty, VPGProjectType::NA);
    
    GETSET(std::wstring, FileContent, L"");
    
    public:
        void SetUp() override
        {
            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJECT_NAME := VCCProjGenerator\r\n";
            makeFileStr += L"DLL_PROJ_NAME := libvpg\r\n";
            makeFileStr += L"EXE_PROJ_NAME := vpg\r\n";
            makeFileStr += L"GTEST_PROJ_NAME := unittest\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            this->_FileContent = makeFileStr;
        }

        void TearDown() override
        {

        }

        std::wstring GetResultStr(const VPGGenerationOption *option) {
            std::wstring projName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
            std::wstring dllName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
            std::wstring exeName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
            std::wstring gtestName = !IsBlank(option->GetProjectName()) ? (L" " + option->GetProjectName()) : L"";
            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJECT_NAME :=" + projName + L"\r\n";
            makeFileStr += L"DLL_PROJ_NAME :=" + dllName + L"\r\n";
            makeFileStr += L"EXE_PROJ_NAME :=" + exeName + L"\r\n";
            makeFileStr += L"GTEST_PROJ_NAME :=" + gtestName + L"\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            return makeFileStr;
        }
};

TEST_F(VPGBaseGenerationManagerTest, Complex)
{
    DECLARE_SPTR(VPGGenerationOption, option);
    option->SetProjectName(L"ProjectName");
    option->SetDllProjectName(L"DllName");
    option->SetExeProjectName(L"ExeName");
    option->SetGtestProjName(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent(), option.get());
    EXPECT_EQ(result, GetResultStr(option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, DLLOnly)
{
    DECLARE_SPTR(VPGGenerationOption, option);
    option->SetProjectName(L"ProjectName");
    option->SetDllProjectName(L"DllName");
    option->SetExeProjectName(L"");
    option->SetGtestProjName(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent(), option.get());
    EXPECT_EQ(result, GetResultStr(option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, EXEOnly)
{
    DECLARE_SPTR(VPGGenerationOption, option);
    option->SetProjectName(L"ProjectName");
    option->SetDllProjectName(L"");
    option->SetExeProjectName(L"ExeName");
    option->SetGtestProjName(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent(), option.get());
    EXPECT_EQ(result, GetResultStr(option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, NoGtest)
{
    DECLARE_SPTR(VPGGenerationOption, option);
    option->SetProjectName(L"ProjectName");
    option->SetDllProjectName(L"DllName");
    option->SetExeProjectName(L"ExeName");
    option->SetGtestProjName(L"");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent(), option.get());
    EXPECT_EQ(result, GetResultStr(option.get()));
}