#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "xml_reader.hpp"

using namespace vcc;

TEST(XMLReaderTest, ParserSimple_String)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L" abc  ";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"");
    EXPECT_EQ(element.GetName(), L"");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"abc");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_SingleTag)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<br/>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"");
    EXPECT_EQ(element.GetName(), L"br");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithString)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<h1>abc</h1>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"");
    EXPECT_EQ(element.GetName(), L"h1");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"abc");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_EmptyTag)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<h2></h2>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"");
    EXPECT_EQ(element.GetName(), L"h2");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_Namespace)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<a:h2></a:h2>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"a");
    EXPECT_EQ(element.GetName(), L"h2");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithEscapeString)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<b:h3>&quot;&amp;&quot;</b:h3>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"b");
    EXPECT_EQ(element.GetName(), L"h3");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"\"&\"");
    EXPECT_EQ(element.GetFullText(), xml);
}

TEST(XMLReaderTest, ParserSimple_TagWithProperties)
{
    std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();
    std::wstring xml = L"<img src=\"img.jpg\" width=\"111\" height=\"222.222\">abc</img>";
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"");
    EXPECT_EQ(element.GetName(), L"img");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)3);
    EXPECT_EQ(element.GetAttributes()->at(0).GetName(), L"src");
    EXPECT_EQ(element.GetAttributes()->at(0).GetValue(), L"img.jpg");
    EXPECT_EQ(element.GetAttributes()->at(1).GetName(), L"width");
    EXPECT_EQ(element.GetAttributes()->at(1).GetValue(), L"111");
    EXPECT_EQ(element.GetAttributes()->at(2).GetName(), L"height");
    EXPECT_EQ(element.GetAttributes()->at(2).GetValue(), L"222.222");
    EXPECT_EQ(element.GetChildren()->size(), (size_t)0);
    EXPECT_EQ(element.GetText(), L"abc");
    EXPECT_EQ(element.GetFullText(), xml);
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
    XMLElement element;
    reader->Parse(xml, element);
    EXPECT_EQ(element.GetNamespace(), L"f");
    EXPECT_EQ(element.GetName(), L"table");
    EXPECT_EQ(element.GetAttributes()->size(), (size_t)0);
    EXPECT_EQ(element.GetChildren()->size(), (size_t)3);
    EXPECT_EQ(element.GetFullText(), L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<f:table>\r\n    <f:td>Alpha</f:td>\r\n    <f:td>&quot;Beta&quot;</f:td>\r\n    <f:td>&amp;gamma</f:td>\r\n</f:table>");
    EXPECT_EQ(element.GetChildren()->at(0).GetNamespace(), L"f");
    EXPECT_EQ(element.GetChildren()->at(0).GetName(), L"td");
    EXPECT_EQ(element.GetChildren()->at(0).GetText(), L"Alpha");
    EXPECT_EQ(element.GetChildren()->at(0).GetFullText(), L"\r\n    <f:td>Alpha</f:td>");
    EXPECT_EQ(element.GetChildren()->at(1).GetNamespace(), L"f");
    EXPECT_EQ(element.GetChildren()->at(1).GetName(), L"td");
    EXPECT_EQ(element.GetChildren()->at(1).GetText(), L"\"Beta\"");
    EXPECT_EQ(element.GetChildren()->at(1).GetFullText(), L"\r\n    <f:td>&quot;Beta&quot;</f:td>");
    EXPECT_EQ(element.GetChildren()->at(2).GetNamespace(), L"f");
    EXPECT_EQ(element.GetChildren()->at(2).GetName(), L"td");
    EXPECT_EQ(element.GetChildren()->at(2).GetText(), L"&gamma");
    EXPECT_EQ(element.GetChildren()->at(2).GetFullText(), L"\r\n    <f:td>&amp;gamma</f:td>");
    EXPECT_EQ(element.GetFullText(), xml);
}
