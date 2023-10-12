#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

void VPGCodeReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, XMLElement &element)
{
    try
    {
        size_t startPos = pos;
        std::wstring endTag = L"</" + (!element.Namespace.empty() ? (element.Namespace + L":") : L"") + element.Name + L">";
        while (pos < xmlData.length())
        {
            if (xmlData.substr(pos).starts_with(endTag)) {
                pos--;
                break;
            }  
            pos++;
        }
        element.Text = pos < xmlData.length() ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}

void VPGCodeReader::ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element)
{
   size_t dataLength = xmlData.length();
    try
    {
        size_t startPos = pos;
        while (pos < dataLength) {
            if (xmlData.substr(pos).starts_with(L"<vcc:")) {
                pos--;
                XMLElement previous;
                previous.Text = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
                previous.FullText = previous.Text;
                element.Children.push_back(previous);

                pos++;
                startPos = pos;

                XMLElement tmp;
                ParseXMLTag(xmlData, pos, tmp);
                tmp.FullText = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
                element.Children.push_back(tmp);

                pos++;
                startPos = pos;
            } else
                pos++;
        }
        if (startPos < xmlData.length() - 1) {
            XMLElement tmp;
            tmp.Text = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
            tmp.FullText = tmp.Text;
            element.Children.push_back(tmp);
        }
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}
