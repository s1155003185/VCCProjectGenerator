#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

VPGCodeReader::VPGCodeReader(std::wstring commandDelimiter)
{
    this->_CommandDelimiter = commandDelimiter;
}

void VPGCodeReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
{
    TRY_CATCH() {
        std::wstring endTag = L"</" + element->GetName() + L">";
        while (pos < xmlData.length())
        {
            if (xmlData.substr(pos).starts_with(endTag)) {
                pos--;
                break;
            }  
            pos++;
        }
    }
}

void VPGCodeReader::ParseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
{
   size_t dataLength = xmlData.length();
    TRY_CATCH() {
        size_t startPos = pos;
        while (pos < dataLength) {
            if (HasPrefix(xmlData, this->_CommandDelimiter, pos) && HasPrefixTrimSpace(xmlData, this->_CommandDelimiter + L"<vcc:", pos)) {
                if (pos > 0) {
                    pos--;
                    DECLARE_SPTR(Xml, previous);
                    previous->SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                    element->InsertChildren(previous);

                    pos++;
                    startPos = pos;
                }

                pos = Find(xmlData, L"<", pos);

                DECLARE_SPTR(Xml, tmp);
                ParseXMLTag(xmlData, pos, tmp);
                tmp->SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                element->InsertChildren(tmp);

                pos++;
                startPos = pos;
            } else
                pos++;
        }
        if (startPos < xmlData.length() - 1) {
            DECLARE_SPTR(Xml, tmp);
            tmp->SetFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
            element->InsertChildren(tmp);
        }
    }
}
