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
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));

            this->_FilePathHpp = vcc::concatPaths({this->getWorkspace(), L"DllFunctions.h"});
            this->_FilePathCpp = vcc::concatPaths({this->getWorkspace(), L"DllFunctions.cpp"});
            
            vcc::copyFile(L"DllFunctions.h", this->_FilePathHpp, true);
            vcc::copyFile(L"DllFunctions.cpp", this->_FilePathCpp, true);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGDllFileGenerationServiceTest, Normal)
{
    auto option = std::make_shared<VPGDllFileGenerationServiceOption>();
    option->setIsGeneratePropertyAccessor(true);
    VPGDllFileGenerationService::GenerateHpp(this->getLogConfig().get(), this->getFilePathHpp(), option.get());
    VPGDllFileGenerationService::GenerateCpp(this->getLogConfig().get(), this->getFilePathCpp(), option.get());

    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathHpp()));
    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathCpp()));

    std::wstring contentHpp = vcc::readFile(this->getFilePathHpp());
    if (!vcc::isContain(contentHpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !vcc::isContain(contentHpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterfaceHeader");  
    }      
    if (!vcc::isContain(contentHpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !vcc::isContain(contentHpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain: dllInterface");
    }

    std::wstring contentCpp = vcc::readFile(this->getFilePathCpp());
    if (!vcc::isContain(contentCpp, L"// <vcc:dllInterfaceHeader gen=\"REPLACE\">") || !vcc::isContain(contentCpp, L"// </vcc:dllInterfaceHeader>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterfaceHeader");
    }
    if (!vcc::isContain(contentCpp, L"// <vcc:dllInterface gen=\"REPLACE\">") || !vcc::isContain(contentCpp, L"// </vcc:dllInterface>")) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain: dllInterface");
    }
}