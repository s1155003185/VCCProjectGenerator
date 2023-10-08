#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

using namespace vcc;

VPGCodeReader::VPGCodeReader(std::wstring commandDelimiter)
{
    this->_CommandDelimiter = commandDelimiter;
}

void VPGCodeReader::ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element)
{
        size_t dataLength = xmlData.length();
        try
        {
            wstring str = L"";
            while (pos < dataLength) {
                str += std::wstring(1, xmlData[pos]);
                
                // if (std::iswspace(xmlData[pos])) {
                //     // nothing to do
                // } else if (pos + 1 < dataLength && xmlData[pos] == L'<' && xmlData[pos + 1] == L'/') {
                //     pos--;
                //     return;
                // }else if (xmlData[pos] == L'<') {
                //     ParseXMLTag(xmlData, pos, element);
                // } else {
                //     size_t endPos = pos;
                //     while (endPos < dataLength) {
                //         if (xmlData[endPos] == L'<') {
                //             endPos--;
                //             break;
                //         }
                //         endPos++;
                //     }
                //     // if not space or tab, then it must be text
                //     element.Text = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
                //     Trim(element.Text);
                //     pos = endPos;
                //     break;
                // }



                GetNextCharPos(xmlData, pos, false);
            }
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
}
