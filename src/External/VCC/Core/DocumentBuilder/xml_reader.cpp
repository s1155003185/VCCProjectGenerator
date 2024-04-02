#include <xml_reader.hpp>

#include <queue>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    // XML ref: https://www.w3.org/TR/xml/
    
    std::wstring XMLReader::_GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
    {
        return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + std::wstring(1, c) + L"': " + msg;
    }

    std::wstring XMLReader::_GetString(const std::wstring &xmlData, size_t &pos)
    {
        size_t endPos = pos;
        try
        {
            if (xmlData.empty() || xmlData[pos] != L'"')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"char is not \""));

            endPos++;
            while (endPos < xmlData.length()) {
                if (xmlData[endPos] == L'\\') {
                    endPos++;
                } else if (xmlData[endPos] == L'"') {
                    break;
                }
                endPos++;
                if (endPos >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"ending \" missing"));
            }
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        // remove head and tail "
        std::wstring  result = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos + 1, (endPos - 1) - (pos + 1) + 1));
        pos = endPos;
        return result;
    }

    std::wstring XMLReader::_GetTag(const std::wstring &xmlData, size_t &pos)
    {
        size_t endPos = pos;
        try
        {
            if (xmlData.empty())
                return L"";

            while (endPos < xmlData.length()) {
                if (!std::iswdigit(xmlData[endPos]) && !std::iswalpha(xmlData[endPos])) {
                    endPos--;
                    break;
                }
                endPos++;
            }
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        std::wstring  result = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
        pos = endPos;
        return result;
    }

    bool XMLReader::_IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos)
    {
        try
        {
            if (xmlData[pos] == L'/') {
                if (pos + 1 >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
                if (xmlData[pos + 1] == L'>') {
                    pos++;
                    return true;
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            } else if (xmlData[pos] == L'>') {
                return true;
            }else if (std::iswspace(xmlData[pos])) {
                GetNextCharacterPos(xmlData, pos, false);
                return _IsNextCharTagEnd(xmlData, pos);
            }
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        return false;
    }

    bool XMLReader::_IsXMLHeader(const std::wstring &xmlData, size_t &pos)
    {
        return xmlData.substr(pos).starts_with(L"<!") || xmlData.substr(pos).starts_with(L"<?");
    }

    void XMLReader::_ParseXMLHeader(const std::wstring &xmlData, size_t &pos)
    {
        try
        {
            // TODO: handle <?...?> and <!...>
            while (pos < xmlData.length()) {
                if (xmlData[pos] == L'"') {
                    _GetString(xmlData, pos);
                }
                if(xmlData[pos] == L'>') {
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
            
    bool XMLReader::ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        try
        {
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"char is not < but " + std::wstring(1, xmlData[pos])));
                
            pos++;
            size_t dataLength = xmlData.length();
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            
            // tag name
            std::wstring tagName = _GetTag(xmlData, pos);
            if (tagName.empty()) {
                if (pos + 1 < dataLength && xmlData[pos + 1] == L'/') {
                    pos--;
                    return false;
                }
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
            }
            pos++;
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));

            if (xmlData[pos] == L':') {
                element->_Namespace = tagName;
                pos++;
                tagName = _GetTag(xmlData, pos);
                if (tagName.empty())
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
                pos++;
                if (pos >= dataLength)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            }
            element->_Name = tagName;
            while (!_IsNextCharTagEnd(xmlData, pos)) {
                DECLARE_SPTR(XMLAttribute, attr);
                attr->_Name = _GetTag(xmlData, pos);
                pos++;
                if (pos >= dataLength || xmlData[pos] != L'=')
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"= missing"));
                pos++;
                attr->_Value = _GetString(xmlData, pos);
                element->_Attributes.push_back(attr);
                pos++;
            }
            // tag end with no ceontent
            return xmlData[pos - 1] != L'/';
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        return false;
    }

    void XMLReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        try
        {
            while (pos < xmlData.length())
            {
                DECLARE_SPTR(XMLElement, child);
                ParseXMLElement(xmlData, pos, child);
                if (!child->_Name.empty()) {
                    element->_Children.push_back(child);
                    
                    std::wstring endTag = L"</" + (!child->_Namespace.empty() ? (child->_Namespace + L":") : L"") + child->_Name + L">";
                    if (!child->_FullText.ends_with(endTag))
                        THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"end tab " + endTag + L" missing"));
                    }
                else {
                    element->_Text = child->_Text;
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
    
    void XMLReader::RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        try
        {
            GetNextCharacterPos(xmlData, pos, true);
            std::wstring endTag = L"</" + (!element->_Namespace.empty() ? (element->_Namespace + L":") : L"") + element->_Name + L">";
            if (!xmlData.substr(pos).starts_with(endTag))
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"end tab " + endTag + L" missing"));
            pos += endTag.length() - 1;
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        } 
    }
    
    void XMLReader::ParseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        try
        {
            if (xmlData.empty())
                return;
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ParserError, _GetErrorMessage(pos, xmlData[pos], L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            // remove xml header
            while (_IsXMLHeader(xmlData, pos)) {
                _ParseXMLHeader(xmlData, pos);
                GetNextCharacterPos(xmlData, pos, false);
            }
            // content
            if (!ParseXMLTagHeader(xmlData, pos, element))
                return;
            pos++;
            ParseXMLTagContent(xmlData, pos, element);
            pos++;
            RemoveXMLTagTail(xmlData, pos, element);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }        
    }

    void XMLReader::ParseXMLElement(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        size_t dataLength = xmlData.length();
        try
        {
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
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    void XMLReader::Parse(const std::wstring &xml, std::shared_ptr<XMLElement> element)
    {
        try
        {
            size_t pos = 0;
            this->Parse(xml, pos, element);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    void XMLReader::Parse(const std::wstring &xml, size_t &pos, std::shared_ptr<XMLElement> element)
    {
        try
        {
            ParseXMLElement(xml, pos, element);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }
}