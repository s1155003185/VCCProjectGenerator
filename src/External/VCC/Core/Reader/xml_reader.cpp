#include <xml_reader.hpp>

#include <queue>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    // XML ref: https://www.w3.org/TR/xml/
    
    std::wstring XMLReader::_GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg)
    {
        return L"Error at position " + std::to_wstring(pos + 1) + L" with char '" + wstring(1, c) + L"': " + msg;
    }

    std::wstring XMLReader::_GetString(const std::wstring &xmlData, size_t &pos)
    {
        size_t endPos = pos;
        try
        {
            if (xmlData.empty() || xmlData[pos] != L'"')
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"char is not \""));

            endPos++;
            while (endPos < xmlData.length()) {
                if (xmlData[endPos] == L'\\') {
                    endPos++;
                } else if (xmlData[endPos] == L'"') {
                    break;
                }
                endPos++;
                if (endPos >= xmlData.length())
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"ending \" missing"));
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
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
                if (xmlData[pos + 1] == L'>') {
                    pos++;
                    return true;
                } else
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            } else if (xmlData[pos] == L'>') {
                return true;
            }else if (std::iswspace(xmlData[pos])) {
                GetNextCharPos(xmlData, pos, false);
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
            
    bool XMLReader::ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        try
        {
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"char is not < but " + std::wstring(1, xmlData[pos])));
                
            pos++;
            size_t dataLength = xmlData.length();
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            
            // tag name
            std::wstring tagName = _GetTag(xmlData, pos);
            if (tagName.empty()) {
                if (pos + 1 < dataLength && xmlData[pos + 1] == L'/') {
                    pos--;
                    return false;
                }
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
            }
            pos++;
            if (pos >= dataLength)
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));

            if (xmlData[pos] == L':') {
                element.Namespace = tagName;
                pos++;
                tagName = _GetTag(xmlData, pos);
                if (tagName.empty())
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
                pos++;
                if (pos >= dataLength)
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            }
            element.Name = tagName;
            while (!_IsNextCharTagEnd(xmlData, pos)) {
                XMLAttribute attr;
                attr.Name = _GetTag(xmlData, pos);
                pos++;
                if (pos >= dataLength || xmlData[pos] != L'=')
                    THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"= missing"));
                pos++;
                attr.Value = _GetString(xmlData, pos);
                element.Attributes.push_back(attr);
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

    void XMLReader::ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        try
        {
            while (pos < xmlData.length())
            {
                XMLElement child;
                ParseXMLElement(xmlData, pos, child);
                if (!child.Name.empty()) {
                    element.Children.push_back(child);
                    
                    std::wstring endTag = L"</" + (!child.Namespace.empty() ? (child.Namespace + L":") : L"") + child.Name + L">";
                    if (!child.FullText.ends_with(endTag))
                        THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"end tab " + endTag + L" missing"));
                    }
                else {
                    element.Text = child.Text;
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
    
    void XMLReader::RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        try
        {
            GetNextCharPos(xmlData, pos, true);
            std::wstring endTag = L"</" + (!element.Namespace.empty() ? (element.Namespace + L":") : L"") + element.Name + L">";
            if (!xmlData.substr(pos).starts_with(endTag))
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"end tab " + endTag + L" missing"));
            pos += endTag.length() - 1;
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        } 
    }
    
    void XMLReader::ParseXMLTag(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        try
        {
            if (xmlData.empty())
                return;
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_MSG(ExceptionType::ReaderError, _GetErrorMessage(pos, xmlData[pos], L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            // remove xml header
            while (_IsXMLHeader(xmlData, pos)) {
                _ParseXMLHeader(xmlData, pos);
                GetNextCharPos(xmlData, pos, false);
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

    void XMLReader::ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element)
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
                    element.Text = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
                    Trim(element.Text);
                    pos = endPos;
                    break;
                }
                GetNextCharPos(xmlData, pos, false);
            }
            element.FullText = pos < dataLength ? xmlData.substr(startPos, pos - startPos + 1) : xmlData.substr(startPos);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    XMLElement XMLReader::Parse(const std::wstring &xml)
    {
        try
        {
            size_t pos = 0;
            return this->Parse(xml, pos);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        XMLElement empty;
        return empty;
    }

    XMLElement XMLReader::Parse(const std::wstring &xml, size_t &pos)
    {
        XMLElement root;
        try
        {
            ParseXMLElement(xml, pos, root);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }        
        return root;
    }
}