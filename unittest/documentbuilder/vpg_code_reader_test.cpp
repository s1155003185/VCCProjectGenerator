#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "vpg_code_reader.hpp"

class VPGCodeReaderTest : public testing::Test 
{
    MANAGER_SPTR_NULL(VPGCodeReader, Reader)
    public:

        void SetUp() override
        {
            _Reader = std::make_shared<VPGCodeReader>(L"//");
        }

        void TearDown() override
        {

        }
};

TEST_F(VPGCodeReaderTest, SimpleString)
{
    std::wstring str = L"abc";
    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), str);
}

TEST_F(VPGCodeReaderTest, Command)
{
    std::wstring str = L"  // comment\r\n";
                    str += L" abc // abc \r\n";
                    str += L" not for vcc // <abc edf/> \r\n";
    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), str);
}

TEST_F(VPGCodeReaderTest, VCCTag)
{
    std::wstring str = L"  // <vcc:vccproj sync=\"FULL\"/>\r\n";
    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)3);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"  ");
    EXPECT_EQ(element->getChildren().at(1)->getName(), L"vcc:vccproj");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getName(), L"sync");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getValue(), L"FULL");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"\r\n");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 3; i++) {
        fullText += element->getChildren().at(i)->getFullText();
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

    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)5);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"    ");
    EXPECT_EQ(element->getChildren().at(1)->getName(), L"vcc:vccproj");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getName(), L"sync");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getValue(), L"FULL");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"\r\n    a\r\n    // <h1>\r\n    content\r\n    // </h1>\r\n    b\r\n    ");
    EXPECT_EQ(element->getChildren().at(3)->getName(), L"vcc:tagB");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getValue(), L"REPLACE");
    EXPECT_EQ(element->getChildren().at(3)->getFullText(), L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element->getChildren().at(4)->getFullText(), L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element->getChildren().at(i)->getFullText();
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

    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)5);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"    ");
    EXPECT_EQ(element->getChildren().at(1)->getName(), L"vcc:vccproj");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getName(), L"sync");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getValue(), L"FULL");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"\r\n    a\r\n    ");
    EXPECT_EQ(element->getChildren().at(3)->getName(), L"vcc:tagA");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getValue(), L"REPLACE");
    EXPECT_EQ(element->getChildren().at(3)->getFullText(), L"// <vcc:tagA action=\"REPLACE\">\r\n    // <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>\r\n    // </vcc:tagA>");
    EXPECT_EQ(element->getChildren().at(4)->getFullText(), L"\r\n    END");
    std::wstring fullText = L"";
    for (size_t i = 0; i < 5; i++) {
        fullText += element->getChildren().at(i)->getFullText();
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

    auto element = std::make_shared<vcc::Xml>();
    this->getReader()->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)7);
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"    ");
    EXPECT_EQ(element->getChildren().at(1)->getName(), L"vcc:vccproj");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getName(), L"sync");
    EXPECT_EQ(element->getChildren().at(1)->getAttributes().at(0)->getValue(), L"FULL");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"// <vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"\r\n    a\r\n    ");
    EXPECT_EQ(element->getChildren().at(3)->getName(), L"vcc:tagA");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(3)->getAttributes().at(0)->getValue(), L"RESERVE");
    EXPECT_EQ(element->getChildren().at(3)->getFullText(), L"// <vcc:tagA action=\"RESERVE\">\r\n    BLOCK A\r\n    // </vcc:tagA>");
    EXPECT_EQ(element->getChildren().at(4)->getFullText(), L"\r\n    b\r\n    ");
    EXPECT_EQ(element->getChildren().at(5)->getName(), L"vcc:tagB");
    EXPECT_EQ(element->getChildren().at(5)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(5)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(5)->getAttributes().at(0)->getValue(), L"REPLACE");
    EXPECT_EQ(element->getChildren().at(5)->getFullText(), L"// <vcc:tagB action=\"REPLACE\">\r\n    BLOCK B\r\n    // </vcc:tagB>");
    EXPECT_EQ(element->getChildren().at(6)->getFullText(), L"\r\n    END");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 7; i++) {
        fullText += element->getChildren().at(i)->getFullText();
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
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(str, element);
    EXPECT_EQ(element->getChildren().size(), (size_t)6);
    EXPECT_EQ(element->getChildren().at(0)->getName(), L"vcc:vccproj");
    EXPECT_EQ(element->getChildren().at(0)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(0)->getAttributes().at(0)->getName(), L"sync");
    EXPECT_EQ(element->getChildren().at(0)->getAttributes().at(0)->getValue(), L"FULL");
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"#\t<vcc:vccproj sync=\"FULL\"/>");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"\r\n\t\ta\r\n");
    EXPECT_EQ(element->getChildren().at(2)->getName(), L"vcc:tagA");
    EXPECT_EQ(element->getChildren().at(2)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(2)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(2)->getAttributes().at(0)->getValue(), L"RESERVE");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"#\t<vcc:tagA action=\"RESERVE\">\r\n\t\tBLOCK A\r\n#\t</vcc:tagA>");
    EXPECT_EQ(element->getChildren().at(3)->getFullText(), L"\r\n\t\tb\r\n");
    EXPECT_EQ(element->getChildren().at(4)->getName(), L"vcc:tagB");
    EXPECT_EQ(element->getChildren().at(4)->getAttributes().size(), (size_t)1);
    EXPECT_EQ(element->getChildren().at(4)->getAttributes().at(0)->getName(), L"action");
    EXPECT_EQ(element->getChildren().at(4)->getAttributes().at(0)->getValue(), L"REPLACE");
    EXPECT_EQ(element->getChildren().at(4)->getFullText(), L"#\t<vcc:tagB action=\"REPLACE\">\r\n\t\tBLOCK B\r\n#\t</vcc:tagB>");
    EXPECT_EQ(element->getChildren().at(5)->getFullText(), L"\r\n\t\tEND");

    std::wstring fullText = L"";
    for (size_t i = 0; i < 6; i++) {
        fullText += element->getChildren().at(i)->getFullText();
    }
    EXPECT_EQ(fullText, str);
}
