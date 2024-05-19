#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_object_type_file_generation_service.hpp"

using namespace vcc;

class VPGObjectTypeFileGenerationServiceTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET(std::wstring, Workspace, L"bin/Debug/VPGObjectTypeFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"object_type.hpp"});
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGObjectTypeFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGObjectTypeFileGenerationSerive::Generate(this->GetLogProperty().get(), this->GetFilePathHpp(), propertyTypes);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    std::wstring content = ReadFile(this->GetFilePathHpp());
    std::wstring expectedResult = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"enum class ObjectType {\r\n"
        L"    NA\r\n"
        L"};\r\n";
    EXPECT_EQ(content, expectedResult);
}

TEST_F(VPGObjectTypeFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGObjectTypeFileGenerationSerive::Generate(this->GetLogProperty().get(), this->GetFilePathHpp(), propertyTypes);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    std::wstring content = ReadFile(this->GetFilePathHpp());
    std::wstring expectedResult = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"enum class ObjectType {\r\n"
        L"    NA,\r\n"
        L"    Abc,\r\n"
        L"    Def\r\n"
        L"};\r\n";
    EXPECT_EQ(content, expectedResult);
}