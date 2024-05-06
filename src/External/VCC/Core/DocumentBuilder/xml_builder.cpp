#include <xml_builder.hpp>

#include <queue>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"
#include "xml.hpp"

namespace vcc
{
    std::wstring XmlBuilder::GetString(const std::wstring &xmlData, size_t &pos) const
    {
        size_t endPos = pos;
        TRY_CATCH(
            if (xmlData.empty() || xmlData[pos] != L'"')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"char is not \""));

            endPos++;
            while (endPos < xmlData.length()) {
                if (xmlData[endPos] == L'\\') {
                    endPos++;
                } else if (xmlData[endPos] == L'"') {
                    break;
                }
                endPos++;
                if (endPos >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"ending \" missing"));
            }
        )
        // remove head and tail "
        std::wstring  result = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos + 1, (endPos - 1) - (pos + 1) + 1));
        pos = endPos;
        return result;
    }

    std::wstring XmlBuilder::GetTag(const std::wstring &xmlData, size_t &pos) const
    {
        size_t endPos = pos;
        TRY_CATCH(
            if (xmlData.empty())
                return L"";

            while (endPos < xmlData.length()) {
                if (!std::iswdigit(xmlData[endPos]) && !std::iswalpha(xmlData[endPos])) {
                    endPos--;
                    break;
                }
                endPos++;
            }
        )
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        std::wstring  result = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
        pos = endPos;
        return result;
    }

    bool XmlBuilder::IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos) const
    {
        TRY_CATCH(
            if (xmlData[pos] == L'/') {
                if (pos + 1 >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"> missing"));
                if (xmlData[pos + 1] == L'>') {
                    pos++;
                    return true;
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"> missing"));
            } else if (xmlData[pos] == L'>') {
                return true;
            }else if (std::iswspace(xmlData[pos])) {
                GetNextCharacterPos(xmlData, pos, false);
                return IsNextCharTagEnd(xmlData, pos);
            }
        )
        return false;
    }

    bool XmlBuilder::IsXMLHeader(const std::wstring &xmlData, size_t &pos) const
    {
        return xmlData.substr(pos).starts_with(L"<!") || xmlData.substr(pos).starts_with(L"<?");
    }

    void XmlBuilder::ParseXMLHeader(const std::wstring &xmlData, size_t &pos) const
    {
        TRY_CATCH(
            // TODO: handle <?...?> and <!...>
            while (pos < xmlData.length()) {
                if (xmlData[pos] == L'"') {
                    GetString(xmlData, pos);
                }
                if(xmlData[pos] == L'>') {
                    break;
                }                    
                pos++;
            }
        )
    }
            
    bool XmlBuilder::ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY_CATCH(
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"char is not < but " + std::wstring(1, xmlData[pos])));
                
            pos++;
            size_t dataLength = xmlData.length();
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"> missing"));
            
            // tag name
            std::wstring tagName = GetTag(xmlData, pos);
            if (tagName.empty()) {
                if (pos + 1 < dataLength && xmlData[pos + 1] == L'/') {
                    pos--;
                    return false;
                }
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"tag name missing"));
            }
            pos++;
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"> missing"));

            if (xmlData[pos] == L':') {
                element->_Name = tagName + L":";
                pos++;
                tagName = GetTag(xmlData, pos);
                if (tagName.empty())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"tag name missing"));
                pos++;
                if (pos >= dataLength)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"> missing"));
            }
            element->_Name += tagName;
            while (!IsNextCharTagEnd(xmlData, pos)) {
                DECLARE_SPTR(XmlAttribute, attr);
                attr->_Name = GetTag(xmlData, pos);
                pos++;
                if (pos >= dataLength || xmlData[pos] != L'=')
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"= missing"));
                pos++;
                attr->_Value = GetString(xmlData, pos);
                element->_Attributes.push_back(attr);
                pos++;
            }
            // tag end with no ceontent
            return xmlData[pos - 1] != L'/';
        )
        return false;
    }

    void XmlBuilder::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY_CATCH(
            while (pos < xmlData.length())
            {
                DECLARE_SPTR(Xml, child);
                ParseXml(xmlData, pos, child);
                if (!child->_Name.empty()) {
                    element->_Children.push_back(child);
                    
                    std::wstring endTag = L"</" + child->_Name + L">";
                    if (!child->_FullText.ends_with(endTag))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"end tab " + endTag + L" missing"));
                    }
                else {
                    element->_Text = child->_Text;
                    break;
                }                   
                pos++;
            }
        )
    }
    
    void XmlBuilder::RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY_CATCH(
            GetNextCharacterPos(xmlData, pos, true);
            std::wstring endTag = L"</" + element->_Name + L">";
            if (!xmlData.substr(pos).starts_with(endTag))
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"end tab " + endTag + L" missing"));
            pos += endTag.length() - 1;
        )
    }
    
    void XmlBuilder::ParseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        TRY_CATCH(
            if (xmlData.empty())
                return;
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, GetErrorMessage(xmlData, pos, L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            // remove xml header
            while (IsXMLHeader(xmlData, pos)) {
                ParseXMLHeader(xmlData, pos);
                GetNextCharacterPos(xmlData, pos, false);
            }
            // content
            if (!ParseXMLTagHeader(xmlData, pos, element))
                return;
            pos++;
            ParseXMLTagContent(xmlData, pos, element);
            pos++;
            RemoveXMLTagTail(xmlData, pos, element);
        )
    }

    void XmlBuilder::ParseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const
    {
        size_t dataLength = xmlData.length();
        TRY_CATCH(
            size_t startPos = pos;
            while (pos < dataLength) {
                if (std::iswspace(xmlData[pos])) {
                    // nothing to do
                } else if (pos + 1 < dataLength && xmlData[pos] == L'<' && xmlData[pos + 1] == L'/') {
                    // parent tag end
                    pos--;
                    return;
                }else if (xmlData[pos] == L'<') {
                    ParseXMLTag(xmlData, pos, element);
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
                    element->_Text = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
                    Trim(element->_Text);
                    pos = endPos;
                    break;
                }
                GetNextCharacterPos(xmlData, pos, false);
            }
            element->_FullText = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
        )
    }
    
    std::wstring XmlBuilder::Serialize(const IDocument *doc) const
    {
        TRY_CATCH(

        )
    }

    void XmlBuilder::Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const
    {
        TRY_CATCH(
            std::shared_ptr<Xml> xmlObject = dynamic_pointer_cast<Xml>(doc);
            assert(xmlObject != nullptr);
            ParseXml(str, pos, xmlObject);
        )
    }
    
    void XmlBuilder::Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const
    {
        TRY_CATCH(
            size_t pos = 0;
            this->Deserialize(str, pos, doc);
        )
    }
}