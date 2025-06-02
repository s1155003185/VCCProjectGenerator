#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "xml_builder.hpp"

TEST(XMLBuilderTest, ParserSimple_String)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L" abc  ";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"");
    EXPECT_EQ(element->GetClosingTag(), L"");
    EXPECT_EQ(element->GetText(), L"abc");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_SingleTag)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<br/>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"br");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<br/>");
    EXPECT_EQ(element->GetClosingTag(), L"");
    EXPECT_EQ(element->GetText(), L"");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithString)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<h1>abc</h1>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"h1");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<h1>");
    EXPECT_EQ(element->GetClosingTag(), L"</h1>");
    EXPECT_EQ(element->GetText(), L"abc");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_EmptyTag)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<h2></h2>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"h2");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<h2>");
    EXPECT_EQ(element->GetClosingTag(), L"</h2>");
    EXPECT_EQ(element->GetText(), L"");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_Namespace)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<a:h2></a:h2>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"a:h2");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<a:h2>");
    EXPECT_EQ(element->GetClosingTag(), L"</a:h2>");
    EXPECT_EQ(element->GetText(), L"");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithEscapeString)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<b:h3>&quot;&amp;&quot;</b:h3>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"b:h3");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<b:h3>");
    EXPECT_EQ(element->GetClosingTag(), L"</b:h3>");
    EXPECT_EQ(element->GetText(), L"\"&\"");
    EXPECT_EQ(element->GetFullText(), xml);
}

TEST(XMLBuilderTest, ParserSimple_TagWithProperties)
{
    std::unique_ptr<vcc::XmlBuilder> reader = std::make_unique<vcc::XmlBuilder>();
    std::wstring xml = L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">abc</img>";
    auto element = std::make_shared<vcc::Xml>();
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"img");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)3);
    EXPECT_EQ(element->GetAttributes().at(0)->GetName(), L"src");
    EXPECT_EQ(element->GetAttributes().at(0)->GetValue(), L"img.jpg");
    EXPECT_EQ(element->GetAttributes().at(1)->GetName(), L"width");
    EXPECT_EQ(element->GetAttributes().at(1)->GetValue(), L"111");
    EXPECT_EQ(element->GetAttributes().at(2)->GetName(), L"height");
    EXPECT_EQ(element->GetAttributes().at(2)->GetValue(), L"222.222");
    EXPECT_EQ(element->GetChildren().size(), (size_t)0);
    EXPECT_EQ(element->GetOpeningTag(), L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">");
    EXPECT_EQ(element->GetClosingTag(), L"</img>");
    EXPECT_EQ(element->GetText(), L"abc");
    EXPECT_EQ(element->GetFullText(), xml);
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
    reader->Deserialize(xml, element);
    EXPECT_EQ(element->GetName(), L"f:table");
    EXPECT_EQ(element->GetAttributes().size(), (size_t)0);
    EXPECT_EQ(element->GetChildren().size(), (size_t)3);
    EXPECT_EQ(element->GetOpeningTag(), L"<f:table>");
    EXPECT_EQ(element->GetClosingTag(), L"</f:table>");
    EXPECT_EQ(element->GetFullText(), L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<f:table>\r\n    <f:td>Alpha</f:td>\r\n    <f:td>&quot;Beta&quot;</f:td>\r\n    <f:td>&amp;gamma</f:td>\r\n</f:table>");
    EXPECT_EQ(element->GetChildren().at(0)->GetName(), L"f:td");
    EXPECT_EQ(element->GetChildren().at(0)->GetOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->GetChildren().at(0)->GetClosingTag(), L"</f:td>");
    EXPECT_EQ(element->GetChildren().at(0)->GetText(), L"Alpha");
    EXPECT_EQ(element->GetChildren().at(0)->GetFullText(), L"\r\n    <f:td>Alpha</f:td>");
    EXPECT_EQ(element->GetChildren().at(1)->GetName(), L"f:td");
    EXPECT_EQ(element->GetChildren().at(1)->GetOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->GetChildren().at(1)->GetClosingTag(), L"</f:td>");
    EXPECT_EQ(element->GetChildren().at(1)->GetText(), L"\"Beta\"");
    EXPECT_EQ(element->GetChildren().at(1)->GetFullText(), L"\r\n    <f:td>&quot;Beta&quot;</f:td>");
    EXPECT_EQ(element->GetChildren().at(2)->GetName(), L"f:td");
    EXPECT_EQ(element->GetChildren().at(2)->GetOpeningTag(), L"<f:td>");
    EXPECT_EQ(element->GetChildren().at(2)->GetClosingTag(), L"</f:td>");
    EXPECT_EQ(element->GetChildren().at(2)->GetText(), L"&gamma");
    EXPECT_EQ(element->GetChildren().at(2)->GetFullText(), L"\r\n    <f:td>&amp;gamma</f:td>");
    EXPECT_EQ(element->GetFullText(), xml);
}
