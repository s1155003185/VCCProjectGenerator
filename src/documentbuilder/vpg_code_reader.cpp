#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

VPGCodeReader::VPGCodeReader(std::wstring commandDelimiter)
{
    this->_CommandDelimiter = commandDelimiter;
}

void VPGCodeReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<vcc::Xml> element) const
{
    TRY
        std::wstring endTag = L"</" + element->getName() + L">";
        while (pos < xmlData.length())
        {
            if (xmlData.substr(pos).starts_with(endTag)) {
                pos--;
                break;
            }  
            pos++;
        }
    CATCH
}

void VPGCodeReader::ParseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<vcc::Xml> element) const
{
   size_t dataLength = xmlData.length();
    TRY
        size_t startPos = pos;
        while (pos < dataLength) {
            if (vcc::isStartWith(xmlData, this->_CommandDelimiter, pos) && vcc::isStartWithTrimSpace(xmlData, this->_CommandDelimiter + L"<vcc:", pos)) {
                if (pos > 0) {
                    pos--;
                    auto previous = std::make_shared<vcc::Xml>();
                    previous->setFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                    element->insertChildren(previous);

                    pos++;
                    startPos = pos;
                }

                pos = vcc::find(xmlData, L"<", pos);

                auto tmp = std::make_shared<vcc::Xml>();
                ParseXMLTag(xmlData, pos, tmp);
                tmp->setFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
                element->insertChildren(tmp);

                pos++;
                startPos = pos;
            } else
                pos++;
        }
        if (startPos < xmlData.length() - 1) {
            auto tmp = std::make_shared<vcc::Xml>();
            tmp->setFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos));
            element->insertChildren(tmp);
        }
    CATCH
}
