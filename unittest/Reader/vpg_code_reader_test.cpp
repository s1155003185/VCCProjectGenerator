#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "vpg_code_reader.hpp"

class VPGCodeReaderTest : public testing::Test 
{
    GETOBJU(VPGCodeReader, Reader, L"//")
    public:

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }
};

// TEST_F(VPGCodeReaderTest, SimpleString)
// {
//     std::wstring str = L"abc";
//     XMLElement element = this->GetReader()->Parse(str);
//     EXPECT_EQ(element.Children.size(), 1);
//     EXPECT_EQ(element.Children.at(0).Text, str);
// }

// TEST_F(VPGCodeReaderTest, Command)
// {
//     std::wstring str = L"  // comment\r\n";
//                     str += L" abc // abc \r\n";
//                     str += L" not for vcc // <abc edf/> \r\n";
//     XMLElement element = this->GetReader()->Parse(str);
//     EXPECT_EQ(element.Children.size(), 1);
//     EXPECT_EQ(element.Children.at(0).Text, str);
// }

// TEST_F(VPGCodeReaderTest, VCCTag)
// {
//     std::wstring str = L"  // <vcc:vccproj sync:\"FULL\"/>\r\n";
//     XMLElement element = this->GetReader()->Parse(str);
//     EXPECT_EQ(element.Children.size(), 3);
//     EXPECT_EQ(element.Children.at(0).Text, L"  // ");
//     EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
//     EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
//     EXPECT_EQ(element.Children.at(1).Attributes.size(), 1);
//     EXPECT_EQ(element.Children.at(1).Attributes.at(1).Name, L"sync");
//     EXPECT_EQ(element.Children.at(1).Attributes.at(1).Value, L"FULL");
//     EXPECT_EQ(element.Children.at(1).Text, L"");
//     EXPECT_EQ(element.Children.at(2).Text, L"\r\n");
// }

// TEST_F(VPGCodeReaderTest, FULL)
// {
//     std::wstring str = L"";
//     str += L"    // <vcc:vccproj sync:\"FULL\"/>\r\n";
//     str += L"    a\r\n";
//     str += L"   // <vcc:tagA action:\"RESERVE\">\r\n";
//     str += L"   BLOCK A\r\n";
//     str += L"   // </vcc:tagA>\r\n";
//     str += L"    b\r\n";
//     str += L"   // <vcc:tagB action:\"REPLACE\">\r\n";
//     str += L"   BLOCK B\r\n";
//     str += L"   </vcc:tagB>\r\n";
//     str += L"   END";

//     XMLElement element = this->GetReader()->Parse(str);
//     EXPECT_EQ(element.Children.size(), 7);
//     EXPECT_EQ(element.Children.at(0).Text, L"  // ");
//     EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
//     EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
//     EXPECT_EQ(element.Children.at(1).Attributes.size(), 1);
//     EXPECT_EQ(element.Children.at(1).Attributes.at(1).Name, L"sync");
//     EXPECT_EQ(element.Children.at(1).Attributes.at(1).Value, L"FULL");
//     EXPECT_EQ(element.Children.at(1).Text, L"");
//     EXPECT_EQ(element.Children.at(2).Text, L"\r\n    a\r\n   // ");
//     EXPECT_EQ(element.Children.at(3).Namespace, L"vcc");
//     EXPECT_EQ(element.Children.at(3).Name, L"tagA");
//     EXPECT_EQ(element.Children.at(3).Attributes.size(), 1);
//     EXPECT_EQ(element.Children.at(3).Attributes.at(1).Name, L"action");
//     EXPECT_EQ(element.Children.at(3).Attributes.at(1).Value, L"RESERVE");
//     EXPECT_EQ(element.Children.at(3).Text, L"\r\n   BLOCK A\r\n   // ");
//     EXPECT_EQ(element.Children.at(4).Text, L"\r\n    b\r\n   // ");
//     EXPECT_EQ(element.Children.at(5).Namespace, L"vcc");
//     EXPECT_EQ(element.Children.at(5).Name, L"tagB");
//     EXPECT_EQ(element.Children.at(5).Attributes.size(), 1);
//     EXPECT_EQ(element.Children.at(5).Attributes.at(1).Name, L"action");
//     EXPECT_EQ(element.Children.at(5).Attributes.at(1).Value, L"REPLACE");
//     EXPECT_EQ(element.Children.at(5).Text, L"\r\n   BLOCK B\r\n   // ");
//     EXPECT_EQ(element.Children.at(6).Text, L"\r\n   END");

// }