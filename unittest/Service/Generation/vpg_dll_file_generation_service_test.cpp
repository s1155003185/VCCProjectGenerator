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
    std::wstring generatedHpp = L"// <vcc:propertyAccessor gen=\"FORCE\">\r\n"
        "// </vcc:propertyAccessor>";
    if (CountSubstring(contentHpp, generatedHpp) != 1)
        EXPECT_EQ(contentHpp, L"Generate Hpp Not Contain:\r\n" + generatedHpp);
    
    std::wstring contentCpp = ReadFile(this->GetFilePathCpp());
    std::wstring generatedCpp = L"// <vcc:propertyAccessor gen=\"FORCE\">\r\n"
        "// </vcc:propertyAccessor>";
    if (CountSubstring(contentCpp, generatedCpp) != 1)
        EXPECT_EQ(contentCpp, L"Generate Cpp Not Contain:\r\n" + generatedCpp);
}