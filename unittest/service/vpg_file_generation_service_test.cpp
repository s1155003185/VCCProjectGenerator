#include <gtest/gtest.h>

#include "vpg_file_generation_service.hpp"

#include <string>

using namespace vcc;

class VPGFileGenerationServiceTest : public testing::Test 
{
    GETSET(std::wstring, DemandHeader, L"// <vcc:vccproj gen=\"DEMAND\"/>\r\n")
    GETSET(std::wstring, SkipHeader, L"// <vcc:vccproj gen=\"SKIP\"/>\r\n")
    GETSET(std::wstring, ForceHeader, L"// <vcc:vccproj gen=\"FORCE\"/>\r\n")

    GETSET(std::wstring, CodeA, L"")
    GETSET(std::wstring, ReplaceCode, L"")

    public:
        void SetUp() override
        {
            this->_CodeA = L"    a\r\n"
                "    // <vcc:tagA gen=\"REPLACE\">\r\n"
                "    BLOCK A\r\n"
                "    // </vcc:tagA>\r\n"
                "    b\r\n"
                "    // <vcc:tagB sync=\"RESERVE\">\r\n"
                "    BLOCK B\r\n"
                "    // </vcc:tagB>\r\n"
                "    c\r\n"
                "    // <vcc:tagC sync=\"REPLACE\">\r\n"
                "    BLOCK C\r\n"
                "    // </vcc:tagC>\r\n"
                "    d\r\n"
                "    // <vcc:tagD sync=\"RESERVE\">\r\n"
                "    BLOCK D\r\n"
                "    // </vcc:tagD>\r\n"
                "    e\r\n";
            
            this->_ReplaceCode = L"line A\r\n"
                "line B\r\n";
        }

        void TearDown() override
        {
        }
};

TEST_F(VPGFileGenerationServiceTest, GetIndent)
{
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L""), L"");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L"   "), L"   ");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L"   \r"), L"   ");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L"   \r\n"), L"   ");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L"   \r\n "), L" ");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L"a"), L"");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L" a"), L" ");
    EXPECT_EQ(VPGFileGenerationService::GetIndent(L" a \r"), L" ");
}

TEST_F(VPGFileGenerationServiceTest, Empty)
{
    std::wstring code = L"";
    std::wstring tagName = L"";
    std::wstring generatedContent = L"";
    std::wstring commandDelimiter = L"//";
    std::wstring result = VPGFileGenerationService::GenerateFileContent(code, tagName, generatedContent, commandDelimiter);
    EXPECT_EQ(result, L"");
}

TEST_F(VPGFileGenerationServiceTest, DEMAND_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetDemandHeader() + this->GetCodeA(), L"vcc:tagA", this->GetReplaceCode(), L"//"),
        this->GetDemandHeader() + L"    a\r\n"
            "    // <vcc:tagA gen=\"REPLACE\">\r\n"
            "    line A\r\n"
            "    line B\r\n"
            "    // </vcc:tagA>\r\n"
            "    b\r\n"
            "    // <vcc:tagB sync=\"RESERVE\">\r\n"
            "    BLOCK B\r\n"
            "    // </vcc:tagB>\r\n"
            "    c\r\n"
            "    // <vcc:tagC sync=\"REPLACE\">\r\n"
            "    BLOCK C\r\n"
            "    // </vcc:tagC>\r\n"
            "    d\r\n"
            "    // <vcc:tagD sync=\"RESERVE\">\r\n"
            "    BLOCK D\r\n"
            "    // </vcc:tagD>\r\n"
            "    e\r\n");
}

TEST_F(VPGFileGenerationServiceTest, DEMAND_MODE_SKIP)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetDemandHeader() + this->GetCodeA(), L"vcc:tagB", this->GetReplaceCode(), L"//"),
        this->GetDemandHeader() + this->GetCodeA());
}

TEST_F(VPGFileGenerationServiceTest, REPLACE_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetForceHeader() + this->GetCodeA(), L"vcc:tagA", this->GetReplaceCode(), L"//"),
        this->GetForceHeader() + L"    a\r\n"
            "    // <vcc:tagA gen=\"REPLACE\">\r\n"
            "    line A\r\n"
            "    line B\r\n"
            "    // </vcc:tagA>\r\n"
            "    b\r\n"
            "    // <vcc:tagB sync=\"RESERVE\">\r\n"
            "    BLOCK B\r\n"
            "    // </vcc:tagB>\r\n"
            "    c\r\n"
            "    // <vcc:tagC sync=\"REPLACE\">\r\n"
            "    BLOCK C\r\n"
            "    // </vcc:tagC>\r\n"
            "    d\r\n"
            "    // <vcc:tagD sync=\"RESERVE\">\r\n"
            "    BLOCK D\r\n"
            "    // </vcc:tagD>\r\n"
            "    e\r\n");
}

TEST_F(VPGFileGenerationServiceTest, REPLACE_MODE_SKIP)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetForceHeader() + this->GetCodeA(), L"vcc:tagB", this->GetReplaceCode(), L"//"),
        this->GetForceHeader() + L"    a\r\n"
            "    // <vcc:tagA gen=\"REPLACE\">\r\n"
            "    BLOCK A\r\n"
            "    // </vcc:tagA>\r\n"
            "    b\r\n"
            "    // <vcc:tagB sync=\"RESERVE\">\r\n"
            "    line A\r\n"
            "    line B\r\n"
            "    // </vcc:tagB>\r\n"
            "    c\r\n"
            "    // <vcc:tagC sync=\"REPLACE\">\r\n"
            "    BLOCK C\r\n"
            "    // </vcc:tagC>\r\n"
            "    d\r\n"
            "    // <vcc:tagD sync=\"RESERVE\">\r\n"
            "    BLOCK D\r\n"
            "    // </vcc:tagD>\r\n"
            "    e\r\n");
}

TEST_F(VPGFileGenerationServiceTest, REPLACE_MODE_NONE)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetForceHeader() + this->GetCodeA(), L"vcc:tagNone", this->GetReplaceCode(), L"//"),
        this->GetForceHeader() + this->GetCodeA() + L"\r\n"
            "    // <vcc:tagNone gen=\"REPLACE\">\r\n"
            "    line A\r\n"
            "    line B\r\n"
            "    // </vcc:tagNone>\r\n");
}

TEST_F(VPGFileGenerationServiceTest, RESERVE_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetSkipHeader() + this->GetCodeA(), L"vcc:tagA", this->GetReplaceCode(), L"//"),
        this->GetSkipHeader() + this->GetCodeA());
}

TEST_F(VPGFileGenerationServiceTest, RESERVE_MODE_SKIP)
{
    EXPECT_EQ(VPGFileGenerationService::GenerateFileContent(this->GetSkipHeader() + this->GetCodeA(), L"vcc:tagB", this->GetReplaceCode(), L"//"),
        this->GetSkipHeader() + this->GetCodeA());
}
