#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_object_type_file_generation_service.hpp"

class VPGObjectTypeFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGObjectTypeFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = vcc::ConcatPaths({this->GetWorkspace(), L"object_type.hpp"});
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGObjectTypeFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGObjectTypeFileGenerationService::Generate(this->GetLogConfig().get(), this->GetFilePathHpp(), propertyTypes);
    EXPECT_TRUE(vcc::IsFilePresent(this->GetFilePathHpp()));
    std::wstring content = vcc::ReadFile(this->GetFilePathHpp());
    std::wstring expectedResult = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        L"#pragma once\r\n"
        "\r\n"
        "enum class ObjectType\r\n"
        "{\r\n"
        "    NA\r\n"
        "    , OperationResult\r\n"
        "    // <vcc:customObjectTypes sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // </vcc:customObjectTypes>\r\n"
        "};\r\n";
    EXPECT_EQ(content, expectedResult);
}

TEST_F(VPGObjectTypeFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGObjectTypeFileGenerationService::Generate(this->GetLogConfig().get(), this->GetFilePathHpp(), propertyTypes);
    EXPECT_TRUE(vcc::IsFilePresent(this->GetFilePathHpp()));
    std::wstring content = vcc::ReadFile(this->GetFilePathHpp());
    std::wstring expectedResult = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        L"#pragma once\r\n"
        "\r\n"
        "enum class ObjectType\r\n"
        "{\r\n"
        "    NA\r\n"
        "    , Abc\r\n"
        "    , Def\r\n"
        "    , OperationResult\r\n"
        "    // <vcc:customObjectTypes sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // </vcc:customObjectTypes>\r\n"
        "};\r\n";
    EXPECT_EQ(content, expectedResult);
}