#include <xml_builder.hpp>

#include <queue>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"
#include "xml.hpp"

namespace vcc
{
    std::wstring XmlBuilder::getString(const std::wstring &xmlData, size_t &pos) const
    {
        size_t endPos = pos;
        TRY
            if (xmlData.empty() || xmlData[pos] != L'"')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"char is not \""));

            endPos++;
            while (endPos < xmlData.length()) {
                if (xmlData[endPos] == L'\\') {
                    endPos++;
                } else if (xmlData[endPos] == L'"') {
                    break;
                }
                endPos++;
                if (endPos >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"ending \" missing"));
            }
        CATCH
        // remove head and tail "
        std::wstring  result = getUnescapeString(EscapeStringType::XML, xmlData.substr(pos + 1, (endPos - 1) - (pos + 1) + 1));
        pos = endPos;
        return result;
    }

    std::wstring XmlBuilder::getTag(const std::wstring &xmlData, size_t &pos) const
    {
        size_t endPos = pos;
        TRY
            if (xmlData.empty())
                return L"";

            while (endPos < xmlData.length()) {
                if (!std::iswdigit(xmlData[endPos]) && !std::iswalpha(xmlData[endPos])) {
                    endPos--;
                    break;
                }
                endPos++;
            }
        CATCH
        std::wstring  result = getUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
        pos = endPos;
        return result;
    }

    bool XmlBuilder::isNextCharTagEnd(const std::wstring &xmlData, size_t &pos) const
    {
        TRY
            if (xmlData[pos] == L'/') {
                if (pos + 1 >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"> missing"));
                if (xmlData[pos + 1] == L'>') {
                    pos++;
                    return true;
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"> missing"));
            } else if (xmlData[pos] == L'>') {
                return true;
            }else if (std::iswspace(xmlData[pos])) {
                getNextCharPos(xmlData, pos, false);
                return isNextCharTagEnd(xmlData, pos);
            }
        CATCH
        return false;
    }

    bool XmlBuilder::isXMLHeader(const std::wstring &xmlData, size_t &pos) const
    {
        return xmlData.substr(pos).starts_with(L"<!") || xmlData.substr(pos).starts_with(L"<?");
    }

    void XmlBuilder::parseXMLHeader(const std::wstring &xmlData, size_t &pos) const
    {
        TRY
            // TODO: handle <?...?> and <!...>
            while (pos < xmlData.length()) {
                if (xmlData[pos] == L'"') {
                    getString(xmlData, pos);
                }
                if(xmlData[pos] == L'>') {
                    break;
                }                    
                pos++;
            }
        CATCH
    }
            
    bool XmlBuilder::parseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            size_t startPos = pos;
            pos++;
            size_t dataLength = xmlData.length();
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"> missing"));
            
            // tag name
            std::wstring tagName = getTag(xmlData, pos);
            if (tagName.empty()) {
                if (pos + 1 < dataLength && xmlData[pos + 1] == L'/') {
                    pos--;
                    return false;
                }
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"tag name missing"));
            }
            pos++;
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"> missing"));

            if (xmlData[pos] == L':') {
                element->_Name = tagName + L":";
                pos++;
                tagName = getTag(xmlData, pos);
                if (tagName.empty())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"tag name missing"));
                pos++;
                if (pos >= dataLength)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"> missing"));
            }
            element->_Name += tagName;
            while (!isNextCharTagEnd(xmlData, pos)) {
                auto attr = std::make_shared<XmlAttribute>();
                attr->_Name = getTag(xmlData, pos);
                pos++;
                if (pos >= dataLength || xmlData[pos] != L'=')
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"= missing"));
                pos++;
                attr->_Value = getString(xmlData, pos);
                element->_Attributes.push_back(attr);
                pos++;
            }
            // tag end with no ceontent
            element->setOpeningTag(xmlData.substr(startPos, pos - startPos + 1));
            return xmlData[pos - 1] != L'/';
        CATCH
        return false;
    }

    void XmlBuilder::parseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY
            while (pos < xmlData.length())
            {
                auto child = std::make_shared<Xml>();
                parseXml(xmlData, pos, child);
                if (!child->_Name.empty()) {
                    element->_Children.push_back(child);
                    
                    std::wstring endTag = L"</" + child->_Name + L">";
                    if (!child->_FullText.ends_with(endTag))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"end tab " + endTag + L" missing"));
                    }
                else {
                    element->_Text = child->_Text;
                    break;
                }                   
                pos++;
            }
        CATCH
    }
    
    void XmlBuilder::removeXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY
            getNextCharPos(xmlData, pos, true);
            std::wstring endTag = L"</" + element->_Name + L">";
            element->setClosingTag(endTag);
            if (!xmlData.substr(pos).starts_with(endTag))
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"end tab " + endTag + L" missing"));
            pos += endTag.length() - 1;
        CATCH
    }
    
    void XmlBuilder::parseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY
            if (xmlData.empty())
                return;
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, getErrorMessage(xmlData, pos, L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            // remove xml header
            while (isXMLHeader(xmlData, pos)) {
                parseXMLHeader(xmlData, pos);
                getNextCharPos(xmlData, pos, false);
            }
            // content
            if (!parseXMLTagHeader(xmlData, pos, element))
                return;
            pos++;
            parseXMLTagContent(xmlData, pos, element);
            pos++;
            removeXMLTagTail(xmlData, pos, element);
        CATCH
    }

    void XmlBuilder::parseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        size_t dataLength = xmlData.length();
        TRY
            size_t startPos = pos;
            while (pos < dataLength) {
                if (std::iswspace(xmlData[pos])) {
                    // nothing to do
                } else if (pos + 1 < dataLength && xmlData[pos] == L'<' && xmlData[pos + 1] == L'/') {
                    // parent tag end
                    pos--;
                    return;
                }else if (xmlData[pos] == L'<') {
                    parseXMLTag(xmlData, pos, element);
                    break;
                } else {
                    size_t endPos = pos;
                    while (endPos < dataLength) {
                        if (xmlData[endPos] == L'<') {
                            endPos--;
                            break;
                        }
                        endPos++;
                    }
                    // if not space or tab, then it must be text
                    element->_Text = getUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
                    trim(element->_Text);
                    pos = endPos;
                    break;
                }
                getNextCharPos(xmlData, pos, false);
            }
            element->_FullText = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
        CATCH
    }
    
    std::wstring XmlBuilder::serialize(const IDocument */*doc*/) const
    {
        TRY

        CATCH
        return L"";
    }

    void XmlBuilder::deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const
    {
        TRY
            auto xmlObject = std::dynamic_pointer_cast<Xml>(doc);
            assert(xmlObject != nullptr);
            parseXml(str, pos, xmlObject);
        CATCH
    }
    
    void XmlBuilder::deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const
    {
        TRY
            size_t pos = 0;
            this->deserialize(str, pos, doc);
        CATCH
    }
}