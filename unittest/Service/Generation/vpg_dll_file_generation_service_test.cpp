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
    GETSET_SPTR(LogProperty, LogProperty);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGDllFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
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
    VPGDllFileGenerationService::GenerateHpp(this->GetLogProperty().get(), this->GetFilePathHpp());
    VPGDllFileGenerationService::GenerateCpp(this->GetLogProperty().get(), this->GetFilePathCpp());

    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring contentHpp = ReadFile(this->GetFilePathHpp());
    if (!(CountSubstring(contentHpp, L"// <vcc:propertyAccessor gen=\"FORCE\">") == 1
        && CountSubstring(contentHpp, L"// </vcc:propertyAccessor>") == 1)) {
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Correct");
    }
    
    std::wstring contentCpp = ReadFile(this->GetFilePathCpp());
    if (!(CountSubstring(contentCpp, L"// <vcc:propertyAccessor gen=\"FORCE\">") == 1
        && CountSubstring(contentCpp, L"// </vcc:propertyAccessor>") == 1)) {
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Correct");
    }
}