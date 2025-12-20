#include "vpg_code_reader.hpp"

#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

VPGCodeReader::VPGCodeReader(std::wstring commandDelimiter) {
    this->_CommandDelimiter = commandDelimiter;
}

void VPGCodeReader::parseXMLTagContent(const std::wstring& xmlData, size_t& pos,
                                       std::shared_ptr<vcc::Xml> element) const {
    TRY std::wstring endTag = L"</" + element->getName() + L">";
    while (pos < xmlData.length()) {
        if (xmlData.substr(pos).starts_with(endTag)) {
            pos--;
            break;
        }
        pos++;
    }
    CATCH
}

void VPGCodeReader::parseXml(const std::wstring& xmlData, size_t& pos,
                             std::shared_ptr<vcc::Xml> element) const {
    size_t dataLength = xmlData.length();
    TRY size_t startPos = pos;
    while (pos < dataLength) {
        if (vcc::isStartWith(xmlData, this->_CommandDelimiter, pos) &&
            vcc::isStartWithTrimSpace(xmlData, this->_CommandDelimiter + L"<vcc:", pos)) {
            if (pos > 0) {
                pos--;
                auto previous = std::make_shared<vcc::Xml>();
                previous->setFullText(pos < dataLength
                                          ? xmlData.substr(startPos, pos - startPos + 1)
                                          : xmlData.substr(startPos));
                element->insertChildren(previous);

                pos++;
                startPos = pos;
            }

            pos = vcc::find(xmlData, L"<", pos);

            // Attempt to handle multi-line commented tag headers like:
            // // <vcc:multi\r\n// action="REPLACE"\r\n//>\r\n
            bool handledMultiLine = false;
            size_t headerScanPos = pos; // position at '<'
            // assemble tag header from consecutive comment lines
            std::wstring assembled;
            size_t scanPos = startPos; // start from command delimiter
            while (scanPos < dataLength) {
                // find line end (CRLF or LF)
                size_t lineEnd = xmlData.find(L"\r\n", scanPos);
                size_t nlLen = 2;
                if (lineEnd == std::wstring::npos) {
                    lineEnd = xmlData.find(L"\n", scanPos);
                    nlLen = (lineEnd == std::wstring::npos) ? 0 : 1;
                }
                if (lineEnd == std::wstring::npos) lineEnd = dataLength;

                std::wstring line = xmlData.substr(scanPos, lineEnd - scanPos);
                size_t delimPos = line.find(this->_CommandDelimiter);
                if (delimPos == std::wstring::npos) break;
                // content after delimiter
                std::wstring after = line.substr(delimPos + this->_CommandDelimiter.length());
                // append (preserve spacing between segments)
                assembled += after;

                // if this line contains '>' then we've reached end of opening tag header
                if (after.find(L'>') != std::wstring::npos) {
                    // we found header end; now try to parse header from assembled
                    size_t localPos = assembled.find(L'<');
                    if (localPos != std::wstring::npos) {
                        auto tmp = std::make_shared<vcc::Xml>();
                        // Manually parse tag name and attributes from assembled to avoid xml_builder parser errors
                        size_t p = localPos + 1; // position after '<'
                        // skip spaces
                        while (p < assembled.size() && std::iswspace(assembled[p])) p++;
                        // parse tag name
                        std::wstring tagName;
                        while (p < assembled.size()) {
                            wchar_t c = assembled[p];
                            if (!std::iswalnum(c) && c != L'_' && c != L':' && c != L'-') break;
                            tagName.push_back(c);
                            p++;
                        }
                        if (tagName.empty()) break;
                        tmp->setName(tagName);
                        // parse attributes
                        while (p < assembled.size()) {
                            // skip spaces
                            while (p < assembled.size() && std::iswspace(assembled[p])) p++;
                            if (p >= assembled.size() || assembled[p] == L'>' || assembled[p] == L'/') break;
                            // read attr name
                            std::wstring attrName;
                            while (p < assembled.size()) {
                                wchar_t c = assembled[p];
                                if (!std::iswalnum(c) && c != L'_' && c != L':' && c != L'-') break;
                                attrName.push_back(c);
                                p++;
                            }
                            // skip spaces
                            while (p < assembled.size() && std::iswspace(assembled[p])) p++;
                            if (p >= assembled.size() || assembled[p] != L'=') break;
                            p++; // skip '='
                            while (p < assembled.size() && std::iswspace(assembled[p])) p++;
                            if (p >= assembled.size() || assembled[p] != L'"') break;
                            // read quoted value
                            p++; // skip '"'
                            std::wstring attrValue;
                            while (p < assembled.size() && assembled[p] != L'"') {
                                if (assembled[p] == L'\\' && p + 1 < assembled.size()) {
                                    // simple unescape
                                    p++;
                                    attrValue.push_back(assembled[p]);
                                } else {
                                    attrValue.push_back(assembled[p]);
                                }
                                p++;
                            }
                            if (p >= assembled.size()) break;
                            // skip closing '"'
                            p++;
                            auto attr = std::make_shared<vcc::XmlAttribute>();
                            attr->setName(attrName);
                            attr->setValue(attrValue);
                            tmp->getAttributes().push_back(attr);
                        }

                        // find closing commented tag in original xmlData. allow optional space between delimiter and '<'
                        size_t searchPos = scanPos + nlLen;
                        size_t closingPos = std::wstring::npos;
                        size_t delimLen = this->_CommandDelimiter.length();
                        std::wstring closingTag = L"</" + tmp->getName() + L">";
                        while (true) {
                            size_t p = xmlData.find(this->_CommandDelimiter, searchPos);
                            if (p == std::wstring::npos) break;
                            size_t q = p + delimLen;
                            // skip optional spaces
                            while (q < xmlData.size() && std::iswspace(xmlData[q])) q++;
                            if (q + closingTag.length() <= xmlData.size() &&
                                xmlData.compare(q, closingTag.length(), closingTag) == 0) {
                                closingPos = p;
                                break;
                            }
                            searchPos = p + 1;
                        }
                        if (closingPos != std::wstring::npos) {
                            // compute end position at the end of the actual closing tag
                            size_t q = closingPos + delimLen;
                            while (q < xmlData.size() && std::iswspace(xmlData[q])) q++;
                            size_t endPos = q + closingTag.length();
                            // create child and set opening/closing tags
                            tmp->setFullText(xmlData.substr(startPos, endPos - startPos));
                            // set opening tag properly from assembled (from '<' to '>')
                            size_t openEnd = assembled.find(L'>', localPos);
                            if (openEnd != std::wstring::npos)
                                tmp->setOpeningTag(assembled.substr(localPos, openEnd - localPos + 1));
                            tmp->setClosingTag(L"</" + tmp->getName() + L">");
                            element->insertChildren(tmp);
                            // move pos to endPos (do not include trailing newline; outer loop will handle it)
                            pos = endPos;
                            startPos = pos;
                            handledMultiLine = true;
                        }
                    }
                    break;
                }

                // advance scanPos to next line
                if (nlLen == 0) break;
                scanPos = (lineEnd == dataLength) ? dataLength : lineEnd + nlLen;
            }

            if (!handledMultiLine) {
                auto tmp = std::make_shared<vcc::Xml>();
                parseXMLTag(xmlData, pos, tmp);
                tmp->setFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1)
                                                  : xmlData.substr(startPos));
                element->insertChildren(tmp);

                pos++;
                startPos = pos;
            }
        } else
            pos++;
    }
    if (startPos < xmlData.length() - 1) {
        auto tmp = std::make_shared<vcc::Xml>();
        tmp->setFullText(pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1)
                                          : xmlData.substr(startPos));
        element->insertChildren(tmp);
    }
    CATCH
}
