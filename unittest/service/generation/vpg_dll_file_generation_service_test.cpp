#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_dll_file_generation_service.hpp"

using namespace vcc;

class VPGDllFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGDllFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"DllFunctions.h"});
            this->_FilePathCpp = ConcatPaths({this->GetWorkspace(), L"DllFunctions.cpp"});
            
            CopyFile(L"DllFunctions.h", this->_FilePathHpp, true);
            CopyFile(L"DllFunctions.cpp", this->_FilePathCpp, true);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGDllFileGenerationServiceTest, Normal)
{
    auto option = std::make_shared<VPGDllFileGenerationServiceOption>();
    option->SetIsGeneratePropertyAccessor(true);
    VPGDllFileGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), option.get());
    VPGDllFileGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetFilePathCpp(), option.get());

    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring contentHpp = ReadFile(this->GetFilePathHpp());
    if (!IsContain(contentHpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !IsContain(contentHpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterfaceHeader");  
    }      
    if (!IsContain(contentHpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !IsContain(contentHpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterface");
    }

    std::wstring contentCpp = ReadFile(this->GetFilePathCpp());
    if (!IsContain(contentCpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !IsContain(contentCpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterfaceHeader");
    }
    if (!IsContain(contentCpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !IsContain(contentCpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterface");
    }
}