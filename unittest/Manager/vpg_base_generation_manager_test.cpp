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
    GET_SPTR(VPGGenerationOption, Option)
    MANAGER(VPGBaseGenerationManager, Manager, _LogProperty, _Option);
    
    GETSET(std::wstring, FileContent, L"");
    
    public:
        void SetUp() override
        {
            _Option->SetWorkspaceSource(L"A");
            _Option->SetWorkspaceDestination(L"B");

            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME := VCCProjGenerator\r\n";
            makeFileStr += L"PROJ_NAME_DLL := libvpg\r\n";
            makeFileStr += L"PROJ_NAME_EXE := vpg\r\n";
            makeFileStr += L"PROJ_NAME_GTEST := unittest\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            this->_FileContent = makeFileStr;
        }

        void TearDown() override
        {

        }

        std::wstring GetResultStr(const VPGGenerationOption *_Option) {
            std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
            std::wstring dllName = !IsBlank(_Option->GetProjectNameDLL()) ? (L" " + _Option->GetProjectNameDLL()) : L"";
            std::wstring exeName = !IsBlank(_Option->GetProjectNameEXE()) ? (L" " + _Option->GetProjectNameEXE()) : L"";
            std::wstring gtestName = !IsBlank(_Option->GetProjectNameGtest()) ? (L" " + _Option->GetProjectNameGtest()) : L"";
            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME :=" + projName + L"\r\n";
            makeFileStr += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
            makeFileStr += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
            makeFileStr += L"PROJ_NAME_GTEST :=" + gtestName + L"\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            return makeFileStr;
        }
};

TEST_F(VPGBaseGenerationManagerTest, Complex)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, DLLOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, EXEOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, NoGtest)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}