#include <gtest/gtest.h>

#include "vpg_file_generation_service.hpp"

#include <string>

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

TEST_F(VPGFileGenerationServiceTest, getIndent)
{
    EXPECT_EQ(VPGFileGenerationService::getIndent(L""), L"");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L"   "), L"   ");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L"   \r"), L"   ");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L"   \r\n"), L"   ");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L"   \r\n "), L" ");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L"a"), L"");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L" a"), L" ");
    EXPECT_EQ(VPGFileGenerationService::getIndent(L" a \r"), L" ");
}

TEST_F(VPGFileGenerationServiceTest, Empty)
{
    std::wstring code = L"";
    std::wstring tagName = L"";
    std::wstring generatedContent = L"";
    std::wstring commandDelimiter = L"//";
    std::wstring result = VPGFileGenerationService::generateFileContent(code, tagName, generatedContent, commandDelimiter);
    EXPECT_EQ(result, L"");
}

TEST_F(VPGFileGenerationServiceTest, DEMAND_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getDemandHeader() + this->getCodeA(), L"vcc:tagA", this->getReplaceCode(), L"//"),
        this->getDemandHeader() + L"    a\r\n"
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
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getDemandHeader() + this->getCodeA(), L"vcc:tagB", this->getReplaceCode(), L"//"),
        this->getDemandHeader() + this->getCodeA());
}

TEST_F(VPGFileGenerationServiceTest, REPLACE_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getForceHeader() + this->getCodeA(), L"vcc:tagA", this->getReplaceCode(), L"//"),
        this->getForceHeader() + L"    a\r\n"
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
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getForceHeader() + this->getCodeA(), L"vcc:tagB", this->getReplaceCode(), L"//"),
        this->getForceHeader() + L"    a\r\n"
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
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getForceHeader() + this->getCodeA(), L"vcc:tagNone", this->getReplaceCode(), L"//"),
        this->getForceHeader() + this->getCodeA() + L"\r\n"
            "    // <vcc:tagNone gen=\"REPLACE\">\r\n"
            "    line A\r\n"
            "    line B\r\n"
            "    // </vcc:tagNone>\r\n");
}

TEST_F(VPGFileGenerationServiceTest, RESERVE_MODE_FORCE)
{
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getSkipHeader() + this->getCodeA(), L"vcc:tagA", this->getReplaceCode(), L"//"),
        this->getSkipHeader() + this->getCodeA());
}

TEST_F(VPGFileGenerationServiceTest, RESERVE_MODE_SKIP)
{
    EXPECT_EQ(VPGFileGenerationService::generateFileContent(this->getSkipHeader() + this->getCodeA(), L"vcc:tagB", this->getReplaceCode(), L"//"),
        this->getSkipHeader() + this->getCodeA());
}
