#include <gtest/gtest.h>

#include "class_macro.hpp"
#include "vpg_file_sync_service.hpp"

#include <string>

using namespace vcc;

class VPGFileSyncServiceTest : public testing::Test 
{    
    GETSET(std::wstring, FullHeader, L"// <vcc:vccproj sync=\"FULL\"/>\r\n")
    GETSET(std::wstring, DemandHeader, L"// <vcc:vccproj sync=\"DEMAND\"/>\r\n")
    GETSET(std::wstring, SkipHeader, L"// <vcc:vccproj sync=\"SKIP\"/>\r\n")
    GETSET(std::wstring, ForceHeader, L"// <vcc:vccproj sync=\"FORCE\"/>\r\n")

    GETSET(std::wstring, StrA, L"")
    GETSET(std::wstring, StrB, L"")

    GETSET(std::wstring, CodeA, L"")
    GETSET(std::wstring, CodeB, L"")

    public:
        void SetUp() override
        {
            this->_StrA = L"a\r\n";
            this->_StrA += L"b\r\n";
            this->_StrB = L"1\r\n";
            this->_StrB += L"2\r\n";

            this->_CodeA = L"    a\r\n";
            this->_CodeA += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
            this->_CodeA += L"// BLOCK A\r\n";
            this->_CodeA += L"// </vcc:tagA>\r\n";
            this->_CodeA += L"    b\r\n";
            this->_CodeA += L"// <vcc:tagB sync=\"REPLACE\">\r\n";
            this->_CodeA += L"// BLOCK B\r\n";
            this->_CodeA += L"// </vcc:tagB>\r\n";
            this->_CodeA += L"    c\r\n";
            this->_CodeA += L"// <vcc:tagC sync=\"REPLACE\">\r\n";
            this->_CodeA += L"// BLOCK C\r\n";
            this->_CodeA += L"// </vcc:tagC>\r\n";
            this->_CodeA += L"    d\r\n";
            this->_CodeA += L"// <vcc:tagD sync=\"RESERVE\">\r\n";
            this->_CodeA += L"// BLOCK D\r\n";
            this->_CodeA += L"// </vcc:tagD>\r\n";
            this->_CodeA += L"    e\r\n";
            this->_CodeA += L"// <vcc:tagE sync=\"REPLACE\">\r\n";
            this->_CodeA += L"// BLOCK E\r\n";
            this->_CodeA += L"// </vcc:tagE>\r\n";

            this->_CodeB = L"    1\r\n";
            this->_CodeB += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
            this->_CodeB += L"// BLOCK 1\r\n";
            this->_CodeB += L"// </vcc:tagA>\r\n";
            this->_CodeB += L"    2\r\n";
            this->_CodeB += L"// <vcc:tagB sync=\"REPLACE\">\r\n";
            this->_CodeB += L"// BLOCK 2\r\n";
            this->_CodeB += L"// </vcc:tagB>\r\n";
            this->_CodeB += L"    3\r\n";
            this->_CodeB += L"// <vcc:tagE sync=\"REPLACE\">\r\n";
            this->_CodeB += L"// BLOCK 3\r\n";
            this->_CodeB += L"// </vcc:tagE>\r\n";
            this->_CodeB += L"    4\r\n";
            this->_CodeB += L"// <vcc:tagD sync=\"RESERVE\">\r\n";
            this->_CodeB += L"// BLOCK 4\r\n";
            this->_CodeB += L"// </vcc:tagD>\r\n";
            this->_CodeB += L"    5\r\n";
            this->_CodeB += L"// <vcc:tagC sync=\"REPLACE\">\r\n";
            this->_CodeB += L"// BLOCK 5\r\n";
            this->_CodeB += L"// </vcc:tagC>\r\n";
        }

        void TearDown() override
        {
        }
};

// Mode
TEST_F(VPGFileSyncServiceTest, DEFAULT_MODE_FORCE)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetStrA(), this->GetStrB(), VPGFileContentSyncMode::Force, L"//"),
                this->GetStrA());
}

TEST_F(VPGFileSyncServiceTest, DEFAULT_MODE_FULL)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetStrA(), this->GetStrB(), VPGFileContentSyncMode::Full, L"//"),
                this->GetStrA());
}

TEST_F(VPGFileSyncServiceTest, DEFAULT_MODE_DEMAND)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetStrA(), this->GetStrB(), VPGFileContentSyncMode::Demand, L"//"),
                this->GetStrB());
}

TEST_F(VPGFileSyncServiceTest, DEFAULT_MODE_SKIP)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetStrA(), this->GetStrB(), VPGFileContentSyncMode::Skip, L"//"),
                this->GetStrB());
}

// Tag - Full and Demand result is the same
TEST_F(VPGFileSyncServiceTest, FULL_RESERVE)
{
    std::wstring srcCode = this->GetFullHeader();
    srcCode += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
    srcCode += L"    int i = 1\r\n";
    srcCode += L"    int j = 1\r\n";
    srcCode += L"    a = L\"a\"\r\n";
    srcCode += L"    b = L\"b\"\r\n";
    srcCode += L"    // commandA = commandA\r\n";
    srcCode += L"    // commandB = commandB\r\n";
    srcCode += L"// </vcc:tagA>\r\n";

    std::wstring destCode = this->GetFullHeader();
    destCode += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
    destCode += L"    int j = 2\r\n";
    destCode += L"    int k = 2\r\n";
    destCode += L"    b = L\"b\"\r\n";
    destCode += L"    c = L\"c\"\r\n";
    destCode += L"    // commandB = commandB\r\n";
    destCode += L"    // commandC = commandC\r\n";
    destCode += L"// </vcc:tagA>\r\n";
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(srcCode, destCode, VPGFileContentSyncMode::NA, L"//"), destCode);
}

TEST_F(VPGFileSyncServiceTest, FULL_REPLACE)
{
    std::wstring srcCode = this->GetFullHeader();
    srcCode += L"// <vcc:tagA sync=\"REPLACE\">\r\n";
    srcCode += L"    int i = 1;\r\n";
    srcCode += L"    int j = 1;\r\n";
    srcCode += L"    a = L\"a\";\r\n";
    srcCode += L"    b = L\"b\";\r\n";
    srcCode += L"    // commandA = commandA\r\n";
    srcCode += L"    // commandB = commandB\r\n";
    srcCode += L"// </vcc:tagA>\r\n";

    std::wstring destCode = this->GetFullHeader();
    destCode += L"// <vcc:tagA sync=\"REPLACE\">\r\n";
    destCode += L"    int j = 2;\r\n";
    destCode += L"    int k = 2;\r\n";
    destCode += L"    b = L\"b\";\r\n";
    destCode += L"    c = L\"c\";\r\n";
    destCode += L"    // commandB = commandB\r\n";
    destCode += L"    // commandC = commandC\r\n";
    destCode += L"// </vcc:tagA>\r\n";
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(srcCode, destCode, VPGFileContentSyncMode::NA, L"//"), srcCode);
}

// Multi Tag
TEST_F(VPGFileSyncServiceTest, FULL_TAG)
{
    std::wstring result = this->GetFullHeader();
    result += L"    a\r\n";
    result += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
    result += L"// BLOCK 1\r\n";
    result += L"// </vcc:tagA>\r\n";
    result += L"    b\r\n";
    result += L"// <vcc:tagB sync=\"REPLACE\">\r\n";
    result += L"// BLOCK B\r\n";
    result += L"// </vcc:tagB>\r\n";
    result += L"    c\r\n";
    result += L"// <vcc:tagC sync=\"REPLACE\">\r\n";
    result += L"// BLOCK C\r\n";
    result += L"// </vcc:tagC>\r\n";
    result += L"    d\r\n";
    result += L"// <vcc:tagD sync=\"RESERVE\">\r\n";
    result += L"// BLOCK 4\r\n";
    result += L"// </vcc:tagD>\r\n";
    result += L"    e\r\n";
    result += L"// <vcc:tagE sync=\"REPLACE\">\r\n";
    result += L"// BLOCK E\r\n";
    result += L"// </vcc:tagE>\r\n";
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetFullHeader() + this->GetCodeA(), this->GetFullHeader() + this->GetCodeB(), VPGFileContentSyncMode::Skip, L"//"),
                result);
}

TEST_F(VPGFileSyncServiceTest, DEMAND_TAG)
{
    std::wstring result = this->GetDemandHeader();
    result += L"    1\r\n";
    result += L"// <vcc:tagA sync=\"RESERVE\">\r\n";
    result += L"// BLOCK 1\r\n";
    result += L"// </vcc:tagA>\r\n";
    result += L"    2\r\n";
    result += L"// <vcc:tagB sync=\"REPLACE\">\r\n";
    result += L"// BLOCK B\r\n";
    result += L"// </vcc:tagB>\r\n";
    result += L"    3\r\n";
    result += L"// <vcc:tagE sync=\"REPLACE\">\r\n";
    result += L"// BLOCK E\r\n";
    result += L"// </vcc:tagE>\r\n";
    result += L"    4\r\n";
    result += L"// <vcc:tagD sync=\"RESERVE\">\r\n";
    result += L"// BLOCK 4\r\n";
    result += L"// </vcc:tagD>\r\n";
    result += L"    5\r\n";
    result += L"// <vcc:tagC sync=\"REPLACE\">\r\n";
    result += L"// BLOCK C\r\n";
    result += L"// </vcc:tagC>\r\n";
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetDemandHeader() + this->GetCodeA(), this->GetDemandHeader() + this->GetCodeB(), VPGFileContentSyncMode::Skip, L"//"),
                result);
}

TEST_F(VPGFileSyncServiceTest, SKIP_TAG)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetSkipHeader() + this->GetCodeA(), this->GetSkipHeader() + this->GetCodeB(), VPGFileContentSyncMode::Skip, L"//"),
                this->GetSkipHeader() + this->GetCodeB());
}

TEST_F(VPGFileSyncServiceTest, FORCE_TAG)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetForceHeader() + this->GetCodeA(), this->GetForceHeader() + this->GetCodeB(), VPGFileContentSyncMode::Skip, L"//"),
                this->GetForceHeader() + this->GetCodeA());
}

TEST_F(VPGFileSyncServiceTest, DifferentTags)
{
    EXPECT_EQ(VPGFileSyncService::SyncFileContent(this->GetSkipHeader() + this->GetCodeA(), this->GetForceHeader() + this->GetCodeB(), VPGFileContentSyncMode::Skip, L"//"),
                this->GetForceHeader() + this->GetCodeA());
}
