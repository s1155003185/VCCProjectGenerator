#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_dll_file_generation_service.hpp"

class VPGDllFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGDllFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = vcc::ConcatPaths({this->GetWorkspace(), L"DllFunctions.h"});
            this->_FilePathCpp = vcc::ConcatPaths({this->GetWorkspace(), L"DllFunctions.cpp"});
            
            vcc::CopyFile(L"DllFunctions.h", this->_FilePathHpp, true);
            vcc::CopyFile(L"DllFunctions.cpp", this->_FilePathCpp, true);
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

    EXPECT_TRUE(vcc::IsFilePresent(this->GetFilePathHpp()));
    EXPECT_TRUE(vcc::IsFilePresent(this->GetFilePathCpp()));

    std::wstring contentHpp = vcc::ReadFile(this->GetFilePathHpp());
    if (!vcc::IsContain(contentHpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !vcc::IsContain(contentHpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterfaceHeader");  
    }      
    if (!vcc::IsContain(contentHpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !vcc::IsContain(contentHpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterface");
    }

    std::wstring contentCpp = vcc::ReadFile(this->GetFilePathCpp());
    if (!vcc::IsContain(contentCpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !vcc::IsContain(contentCpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterfaceHeader");
    }
    if (!vcc::IsContain(contentCpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !vcc::IsContain(contentCpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterface");
    }
}