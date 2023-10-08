#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "xml_reader.hpp"

using namespace vcc;

TEST(XMLReaderTest, ParserSimple_String)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L" abc  ";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"");
    EXPECT_EQ(element.Name, L"");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"abc");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_SingleTag)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<br/>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"");
    EXPECT_EQ(element.Name, L"br");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithString)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<h1>abc</h1>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"");
    EXPECT_EQ(element.Name, L"h1");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"abc");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_EmptyTag)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<h2></h2>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"");
    EXPECT_EQ(element.Name, L"h2");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_Namespace)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<a:h2></a:h2>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"a");
    EXPECT_EQ(element.Name, L"h2");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithEscapeString)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<b:h3>&quot;&amp;&quot;</b:h3>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"b");
    EXPECT_EQ(element.Name, L"h3");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"\"&\"");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithProperties)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">abc</img>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"");
    EXPECT_EQ(element.Name, L"img");
    EXPECT_EQ(element.Attributes.size(), (size_t)3);
    EXPECT_EQ(element.Attributes.at(0).Name, L"src");
    EXPECT_EQ(element.Attributes.at(0).Value, L"img.jpg");
    EXPECT_EQ(element.Attributes.at(1).Name, L"width");
    EXPECT_EQ(element.Attributes.at(1).Value, L"111");
    EXPECT_EQ(element.Attributes.at(2).Name, L"height");
    EXPECT_EQ(element.Attributes.at(2).Value, L"222.222");
    EXPECT_EQ(element.Children.size(), (size_t)0);
    EXPECT_EQ(element.Text, L"abc");
    EXPECT_EQ(element.FullText, xml);
}

TEST(XMLReaderTest, Full) 
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
    xml += L"<f:table>\r\n";
    xml += L"    <f:td>Alpha</f:td>\r\n";
    xml += L"    <f:td>&quot;Beta&quot;</f:td>\r\n";
    xml += L"    <f:td>&amp;gamma</f:td>\r\n";
    xml += L"</f:table>";
    XMLElement element = reader->Parse(xml);
    EXPECT_EQ(element.Namespace, L"f");
    EXPECT_EQ(element.Name, L"table");
    EXPECT_EQ(element.Attributes.size(), (size_t)0);
    EXPECT_EQ(element.Children.size(), (size_t)3);
    EXPECT_EQ(element.FullText, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<f:table>\r\n    <f:td>Alpha</f:td>\r\n    <f:td>&quot;Beta&quot;</f:td>\r\n    <f:td>&amp;gamma</f:td>\r\n</f:table>");
    EXPECT_EQ(element.Children.at(0).Namespace, L"f");
    EXPECT_EQ(element.Children.at(0).Name, L"td");
    EXPECT_EQ(element.Children.at(0).Text, L"Alpha");
    EXPECT_EQ(element.Children.at(0).FullText, L"\r\n    <f:td>Alpha</f:td>");
    EXPECT_EQ(element.Children.at(1).Namespace, L"f");
    EXPECT_EQ(element.Children.at(1).Name, L"td");
    EXPECT_EQ(element.Children.at(1).Text, L"\"Beta\"");
    EXPECT_EQ(element.Children.at(1).FullText, L"\r\n    <f:td>&quot;Beta&quot;</f:td>");
    EXPECT_EQ(element.Children.at(2).Namespace, L"f");
    EXPECT_EQ(element.Children.at(2).Name, L"td");
    EXPECT_EQ(element.Children.at(2).Text, L"&gamma");
    EXPECT_EQ(element.Children.at(2).FullText, L"\r\n    <f:td>&amp;gamma</f:td>");
    EXPECT_EQ(element.FullText, xml);
}
