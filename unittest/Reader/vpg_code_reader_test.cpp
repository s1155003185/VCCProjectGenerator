#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "vpg_code_reader.hpp"

class VPGCodeReaderTest : public testing::Test 
{
    GETUPTR(VPGCodeReader, Reader, L"//")
    public:

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }
};

TEST_F(VPGCodeReaderTest, SimpleString)
{
    std::wstring str = L"abc";
    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(0).FullText, str);
}

TEST_F(VPGCodeReaderTest, Command)
{
    std::wstring str = L"  // comment\r\n";
                    str += L" abc // abc \r\n";
                    str += L" not for vcc // <abc edf/> \r\n";
    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(0).FullText, str);
}

TEST_F(VPGCodeReaderTest, VCCTag)
{
    std::wstring str = L"  // <vcc:vccproj sync=\"FULL\"/>\r\n";
    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)3);
    EXPECT_EQ(element.Children.at(0).FullText, L"  ");
    EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
    EXPECT_EQ(element.Children.at(1).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Name, L"sync");
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Value, L"FULL");
    EXPECT_EQ(element.Children.at(1).FullText, L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element.Children.at(2).FullText, L"\r\n");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 3; i++) {
        fullText += element.Children.at(i).FullText;
    }
    EXPECT_EQ(fullText, str);

}

TEST_F(VPGCodeReaderTest, Mixed)
{
    std::wstring str = L"";
    str += L"    // <vcc:vccproj sync=\"FULL\"/>\r\n";
    str += L"    a\r\n";
    str += L"    // <h1>\r\n";
    str += L"    content\r\n";
    str += L"    // </h1>\r\n";
    str += L"    b\r\n";
    str += L"    // <vcc:tagB action=\"REPLACE\">\r\n";
    str += L"    BLOCK B\r\n";
    str += L"    // </vcc:tagB>\r\n";
    str += L"    END";

    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)5);
    EXPECT_EQ(element.Children.at(0).FullText, L"    ");
    EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
    EXPECT_EQ(element.Children.at(1).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Name, L"sync");
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Value, L"FULL");
    EXPECT_EQ(element.Children.at(1).FullText, L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element.Children.at(2).FullText, L"\r\n    a\r\n    // <h1>\r\n    content\r\n    // </h1>\r\n    b\r\n    ");
    EXPECT_EQ(element.Children.at(3).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(3).Name, L"tagB");
    EXPECT_EQ(element.Children.at(3).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Value, L"REPLACE");
    EXPECT_EQ(element.Children.at(3).FullText, L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element.Children.at(4).FullText, L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element.Children.at(i).FullText;
    }
    EXPECT_EQ(fullText, str);
}

TEST_F(VPGCodeReaderTest, Nested)
{
    std::wstring str = L"";
    str += L"    // <vcc:vccproj sync=\"FULL\"/>\r\n";
    str += L"    a\r\n";
    str += L"    // <vcc:tagA action=\"REPLACE\">\r\n";
    str += L"    // <vcc:tagB action=\"REPLACE\">\r\n";
    str += L"    BLOCK B\r\n";
    str += L"    // </vcc:tagB>\r\n";
    str += L"    // </vcc:tagA>\r\n";
    str += L"    END";

    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)5);
    EXPECT_EQ(element.Children.at(0).FullText, L"    ");
    EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
    EXPECT_EQ(element.Children.at(1).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Name, L"sync");
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Value, L"FULL");
    EXPECT_EQ(element.Children.at(1).FullText, L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element.Children.at(2).FullText, L"\r\n    a\r\n    ");
    EXPECT_EQ(element.Children.at(3).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(3).Name, L"tagA");
    EXPECT_EQ(element.Children.at(3).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Value, L"REPLACE");
    EXPECT_EQ(element.Children.at(3).FullText, L"// <vcc:tagA action=\"REPLACE\">\r\n    // <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>\r\n    // </vcc:tagA>");
    EXPECT_EQ(element.Children.at(4).FullText, L"\r\n    END");
    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element.Children.at(i).FullText;
    }
    EXPECT_EQ(fullText, str);
}

TEST_F(VPGCodeReaderTest, FULL)
{
    std::wstring str = L"";
    str += L"    // <vcc:vccproj sync=\"FULL\"/>\r\n";
    str += L"    a\r\n";
    str += L"    // <vcc:tagA action=\"RESERVE\">\r\n";
    str += L"    BLOCK A\r\n";
    str += L"    // </vcc:tagA>\r\n";
    str += L"    b\r\n";
    str += L"    // <vcc:tagB action=\"REPLACE\">\r\n";
    str += L"    BLOCK B\r\n";
    str += L"    // </vcc:tagB>\r\n";
    str += L"    END";

    XMLElement element = this->GetReader()->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)7);
    EXPECT_EQ(element.Children.at(0).FullText, L"    ");
    EXPECT_EQ(element.Children.at(1).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(1).Name, L"vccproj");
    EXPECT_EQ(element.Children.at(1).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Name, L"sync");
    EXPECT_EQ(element.Children.at(1).Attributes.at(0).Value, L"FULL");
    EXPECT_EQ(element.Children.at(1).FullText, L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element.Children.at(2).FullText, L"\r\n    a\r\n    ");
    EXPECT_EQ(element.Children.at(3).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(3).Name, L"tagA");
    EXPECT_EQ(element.Children.at(3).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(3).Attributes.at(0).Value, L"RESERVE");
    EXPECT_EQ(element.Children.at(3).FullText, L"// <vcc:tagA action=\"RESERVE\">\r\n    BLOCK A\r\n    // </vcc:tagA>");
    EXPECT_EQ(element.Children.at(4).FullText, L"\r\n    b\r\n    ");
    EXPECT_EQ(element.Children.at(5).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(5).Name, L"tagB");
    EXPECT_EQ(element.Children.at(5).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(5).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(5).Attributes.at(0).Value, L"REPLACE");
    EXPECT_EQ(element.Children.at(5).FullText, L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element.Children.at(6).FullText, L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 7; i++) {
        fullText += element.Children.at(i).FullText;
    }
    EXPECT_EQ(fullText, str);
}

TEST_F(VPGCodeReaderTest, ReadMe)
{
    std::wstring str = L"";
    str += L"#\t<vcc:vccproj sync=\"FULL\"/>\r\n";
    str += L"\t\ta\r\n";
    str += L"#\t<vcc:tagA action=\"RESERVE\">\r\n";
    str += L"\t\tBLOCK A\r\n";
    str += L"#\t</vcc:tagA>\r\n";
    str += L"\t\tb\r\n";
    str += L"#\t<vcc:tagB action=\"REPLACE\">\r\n";
    str += L"\t\tBLOCK B\r\n";
    str += L"#\t</vcc:tagB>\r\n";
    str += L"\t\tEND";

    std::unique_ptr<VPGCodeReader> reader = std::make_unique<VPGCodeReader>(L"#");

    XMLElement element = reader->Parse(str);
    EXPECT_EQ(element.Children.size(), (size_t)6);
    EXPECT_EQ(element.Children.at(0).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(0).Name, L"vccproj");
    EXPECT_EQ(element.Children.at(0).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(0).Attributes.at(0).Name, L"sync");
    EXPECT_EQ(element.Children.at(0).Attributes.at(0).Value, L"FULL");
    EXPECT_EQ(element.Children.at(0).FullText, L"#\t<vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element.Children.at(1).FullText, L"\r\n\t\ta\r\n");
    EXPECT_EQ(element.Children.at(2).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(2).Name, L"tagA");
    EXPECT_EQ(element.Children.at(2).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(2).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(2).Attributes.at(0).Value, L"RESERVE");
    EXPECT_EQ(element.Children.at(2).FullText, L"#\t<vcc:tagA action=\"RESERVE\">\r\n\t\tBLOCK A\r\n#\t</vcc:tagA>");
    EXPECT_EQ(element.Children.at(3).FullText, L"\r\n\t\tb\r\n");
    EXPECT_EQ(element.Children.at(4).Namespace, L"vcc");
    EXPECT_EQ(element.Children.at(4).Name, L"tagB");
    EXPECT_EQ(element.Children.at(4).Attributes.size(), (size_t)1);
    EXPECT_EQ(element.Children.at(4).Attributes.at(0).Name, L"action");
    EXPECT_EQ(element.Children.at(4).Attributes.at(0).Value, L"REPLACE");
    EXPECT_EQ(element.Children.at(4).FullText, L"#\t<vcc:tagB action=\"REPLACE\">\r\n\t\tBLOCK B\r\n#\t</vcc:tagB>");
    EXPECT_EQ(element.Children.at(5).FullText, L"\r\n\t\tEND");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 6; i++) {
        fullText += element.Children.at(i).FullText;
    }
    EXPECT_EQ(fullText, str);
}
