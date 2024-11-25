#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"

class VPGCodeReaderTest : public testing::Test 
{
    MANAGER_SPTR(VPGCodeReader, Reader, L"//")
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
    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), str);
}

TEST_F(VPGCodeReaderTest, Command)
{
    std::wstring str = L"  // comment\r\n";
                    str += L" abc // abc \r\n";
                    str += L" not for vcc // <abc edf/> \r\n";
    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), str);
}

TEST_F(VPGCodeReaderTest, VCCTag)
{
    std::wstring str = L"  // <vcc:vccproj sync=\"FULL\"/>\r\n";
    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)3);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"  ");
    EXPECT_EQ(element->GetChildren().at(1)->GetName(), L"vcc:vccproj");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetName(), L"sync");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetValue(), L"FULL");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"\r\n");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 3; i++) {
        fullText += element->GetChildren().at(i)->GetFullText();
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

    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)5);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"    ");
    EXPECT_EQ(element->GetChildren().at(1)->GetName(), L"vcc:vccproj");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetName(), L"sync");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetValue(), L"FULL");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"\r\n    a\r\n    // <h1>\r\n    content\r\n    // </h1>\r\n    b\r\n    ");
    EXPECT_EQ(element->GetChildren().at(3)->GetName(), L"vcc:tagB");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetValue(), L"REPLACE");
    EXPECT_EQ(element->GetChildren().at(3)->GetFullText(), L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element->GetChildren().at(4)->GetFullText(), L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element->GetChildren().at(i)->GetFullText();
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

    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)5);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"    ");
    EXPECT_EQ(element->GetChildren().at(1)->GetName(), L"vcc:vccproj");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetName(), L"sync");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetValue(), L"FULL");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"\r\n    a\r\n    ");
    EXPECT_EQ(element->GetChildren().at(3)->GetName(), L"vcc:tagA");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetValue(), L"REPLACE");
    EXPECT_EQ(element->GetChildren().at(3)->GetFullText(), L"// <vcc:tagA action=\"REPLACE\">\r\n    // <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>\r\n    // </vcc:tagA>");
    EXPECT_EQ(element->GetChildren().at(4)->GetFullText(), L"\r\n    END");
    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element->GetChildren().at(i)->GetFullText();
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

    DECLARE_SPTR(Xml, element);
    this->GetReader()->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)7);
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"    ");
    EXPECT_EQ(element->GetChildren().at(1)->GetName(), L"vcc:vccproj");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetName(), L"sync");
    EXPECT_EQ(element->GetChildren().at(1)->GetAttributes().at(0)->GetValue(), L"FULL");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"\r\n    a\r\n    ");
    EXPECT_EQ(element->GetChildren().at(3)->GetName(), L"vcc:tagA");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(3)->GetAttributes().at(0)->GetValue(), L"RESERVE");
    EXPECT_EQ(element->GetChildren().at(3)->GetFullText(), L"// <vcc:tagA action=\"RESERVE\">\r\n    BLOCK A\r\n    // </vcc:tagA>");
    EXPECT_EQ(element->GetChildren().at(4)->GetFullText(), L"\r\n    b\r\n    ");
    EXPECT_EQ(element->GetChildren().at(5)->GetName(), L"vcc:tagB");
    EXPECT_EQ(element->GetChildren().at(5)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(5)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(5)->GetAttributes().at(0)->GetValue(), L"REPLACE");
    EXPECT_EQ(element->GetChildren().at(5)->GetFullText(), L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element->GetChildren().at(6)->GetFullText(), L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 7; i++) {
        fullText += element->GetChildren().at(i)->GetFullText();
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
    DECLARE_SPTR(Xml, element);
    reader->Deserialize(str, element);
    EXPECT_EQ(element->GetChildren().size(), (size_t)6);
    EXPECT_EQ(element->GetChildren().at(0)->GetName(), L"vcc:vccproj");
    EXPECT_EQ(element->GetChildren().at(0)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(0)->GetAttributes().at(0)->GetName(), L"sync");
    EXPECT_EQ(element->GetChildren().at(0)->GetAttributes().at(0)->GetValue(), L"FULL");
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"#\t<vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"\r\n\t\ta\r\n");
    EXPECT_EQ(element->GetChildren().at(2)->GetName(), L"vcc:tagA");
    EXPECT_EQ(element->GetChildren().at(2)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(2)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(2)->GetAttributes().at(0)->GetValue(), L"RESERVE");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"#\t<vcc:tagA action=\"RESERVE\">\r\n\t\tBLOCK A\r\n#\t</vcc:tagA>");
    EXPECT_EQ(element->GetChildren().at(3)->GetFullText(), L"\r\n\t\tb\r\n");
    EXPECT_EQ(element->GetChildren().at(4)->GetName(), L"vcc:tagB");
    EXPECT_EQ(element->GetChildren().at(4)->GetAttributes().size(), (size_t)1);
    EXPECT_EQ(element->GetChildren().at(4)->GetAttributes().at(0)->GetName(), L"action");
    EXPECT_EQ(element->GetChildren().at(4)->GetAttributes().at(0)->GetValue(), L"REPLACE");
    EXPECT_EQ(element->GetChildren().at(4)->GetFullText(), L"#\t<vcc:tagB action=\"REPLACE\">\r\n\t\tBLOCK B\r\n#\t</vcc:tagB>");
    EXPECT_EQ(element->GetChildren().at(5)->GetFullText(), L"\r\n\t\tEND");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 6; i++) {
        fullText += element->GetChildren().at(i)->GetFullText();
    }
    EXPECT_EQ(fullText, str);
}
