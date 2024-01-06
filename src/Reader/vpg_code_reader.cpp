#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

VPGCodeReader::VPGCodeReader(std::wstring commandDelimiter)
{
    this->_CommandDelimiter = commandDelimiter;
}

void VPGCodeReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, XMLElement &element)
{
    try
    {
        std::wstring endTag = L"</" + (!element.GetNamespace().empty() ? (element.GetNamespace() + L":") : L"") + element.GetName() + L">";
        while (pos < xmlData.length())
        {
            if (xmlData.substr(pos).starts_with(endTag)) {
                pos--;
                break;
            }  
            pos++;
        }
    }
    catch(const std::exception& e)
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
            if (HasPrefix(xmlData, this->_CommandDelimiter, pos) && HasPrefixTrimSpace(xmlData, this->_CommandDelimiter + L"<vcc:", pos)) {
                if (pos > 0) {
                    pos--;
                    XMLElement previous;
                    previous.SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                    element.InsertChildren(previous);

                    pos++;
                    startPos = pos;
                }

                pos = xmlData.find(L"<", pos);

                XMLElement tmp;
                ParseXMLTag(xmlData, pos, tmp);
                tmp.SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                element.InsertChildren(tmp);

                pos++;
                startPos = pos;
            } else
                pos++;
        }
        if (startPos < xmlData.length() - 1) {
            XMLElement tmp;
            tmp.SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
            element.InsertChildren(tmp);
        }
    }
    catch(const std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}
