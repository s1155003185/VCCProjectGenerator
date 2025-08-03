#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "xml_builder.hpp"

TEST(XMLBuilderTest, ParserSimple_String)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L" abc  ";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"");
    EXPECT_EQ(element->getClosingTag(), L"");
    EXPECT_EQ(element->getText(), L"abc");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_SingleTag)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<br/>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"br");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<br/>");
    EXPECT_EQ(element->getClosingTag(), L"");
    EXPECT_EQ(element->getText(), L"");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithString)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<h1>abc</h1>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"h1");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<h1>");
    EXPECT_EQ(element->getClosingTag(), L"</h1>");
    EXPECT_EQ(element->getText(), L"abc");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_EmptyTag)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<h2></h2>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"h2");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<h2>");
    EXPECT_EQ(element->getClosingTag(), L"</h2>");
    EXPECT_EQ(element->getText(), L"");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_Namespace)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<a:h2></a:h2>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"a:h2");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<a:h2>");
    EXPECT_EQ(element->getClosingTag(), L"</a:h2>");
    EXPECT_EQ(element->getText(), L"");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithEscapeString)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<b:h3>&quot;&amp;&quot;</b:h3>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"b:h3");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<b:h3>");
    EXPECT_EQ(element->getClosingTag(), L"</b:h3>");
    EXPECT_EQ(element->getText(), L"\"&\"");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithProperties)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">abc</img>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"img");
    EXPECT_EQ(element->getAttributes().size(), (size_t)3);
    EXPECT_EQ(element->getAttributes().at(0)->getName(), L"src");
    EXPECT_EQ(element->getAttributes().at(0)->getValue(), L"img.jpg");
    EXPECT_EQ(element->getAttributes().at(1)->getName(), L"width");
    EXPECT_EQ(element->getAttributes().at(1)->getValue(), L"111");
    EXPECT_EQ(element->getAttributes().at(2)->getName(), L"height");
    EXPECT_EQ(element->getAttributes().at(2)->getValue(), L"222.222");
    EXPECT_EQ(element->getChildren().size(), (size_t)0);
    EXPECT_EQ(element->getOpeningTag(), L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">");
    EXPECT_EQ(element->getClosingTag(), L"</img>");
    EXPECT_EQ(element->getText(), L"abc");
    EXPECT_EQ(element->getFullText(), xml);
}

TEST(XMLBuilderTest, Full) 
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
    xml += L"<f:table>\r\n";
    xml += L"    <f:td>Alpha</f:td>\r\n";
    xml += L"    <f:td>&quot;Beta&quot;</f:td>\r\n";
    xml += L"    <f:td>&amp;gamma</f:td>\r\n";
    xml += L"</f:table>";
    auto element = std::make_shared<vcc::Xml>();
    reader->deserialize(xml, element);
    EXPECT_EQ(element->getName(), L"f:table");
    EXPECT_EQ(element->getAttributes().size(), (size_t)0);
    EXPECT_EQ(element->getChildren().size(), (size_t)3);
    EXPECT_EQ(element->getOpeningTag(), L"<f:table>");
    EXPECT_EQ(element->getClosingTag(), L"</f:table>");
    EXPECT_EQ(element->getFullText(), L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<f:table>\r\n    <f:td>Alpha</f:td>\r\n    <f:td>&quot;Beta&quot;</f:td>\r\n    <f:td>&amp;gamma</f:td>\r\n</f:table>");
    EXPECT_EQ(element->getChildren().at(0)->getName(), L"f:td");
    EXPECT_EQ(element->getChildren().at(0)->getOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->getChildren().at(0)->getClosingTag(), L"</f:td>");
    EXPECT_EQ(element->getChildren().at(0)->getText(), L"Alpha");
    EXPECT_EQ(element->getChildren().at(0)->getFullText(), L"\r\n    <f:td>Alpha</f:td>");
    EXPECT_EQ(element->getChildren().at(1)->getName(), L"f:td");
    EXPECT_EQ(element->getChildren().at(1)->getOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->getChildren().at(1)->getClosingTag(), L"</f:td>");
    EXPECT_EQ(element->getChildren().at(1)->getText(), L"\"Beta\"");
    EXPECT_EQ(element->getChildren().at(1)->getFullText(), L"\r\n    <f:td>&quot;Beta&quot;</f:td>");
    EXPECT_EQ(element->getChildren().at(2)->getName(), L"f:td");
    EXPECT_EQ(element->getChildren().at(2)->getOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->getChildren().at(2)->getClosingTag(), L"</f:td>");
    EXPECT_EQ(element->getChildren().at(2)->getText(), L"&gamma");
    EXPECT_EQ(element->getChildren().at(2)->getFullText(), L"\r\n    <f:td>&amp;gamma</f:td>");
    EXPECT_EQ(element->getFullText(), xml);
}
