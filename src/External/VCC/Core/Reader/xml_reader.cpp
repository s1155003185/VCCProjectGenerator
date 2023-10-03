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
                THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"char is not \""));

            endPos++;
            while (endPos < xmlData.length()) {
                if (xmlData[endPos] == L'\\') {
                    endPos++;
                } else if (xmlData[endPos] == L'"') {
                    break;
                }
                endPos++;
                if (endPos >= xmlData.length())
                    THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"ending \" missing"));
            }
        }
        catch(std::exception& e)
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
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        std::wstring  result = GetUnescapeString(EscapeStringType::XML, xmlData.substr(pos, endPos - pos + 1));
        pos = endPos;
        return result;
    }

    void XMLReader::_ToNextChar(const std::wstring &xmlData, size_t &pos)
    {
        pos++;
        while (pos < xmlData.length()) {
            if (!std::iswspace(xmlData[pos]))
                break;
            pos++;
        }
    }

    bool XMLReader::_IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos)
    {
        try
        {
            if (xmlData[pos] == L'/') {
                if (pos + 1 >= xmlData.length())
                    THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
                if (xmlData[pos + 1] == L'>') {
                    pos++;
                    return true;
                } else
                    THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            } else if (xmlData[pos] == L'>') {
                return true;
            }else if (std::iswspace(xmlData[pos])) {
                _ToNextChar(xmlData, pos);
                return _IsNextCharTagEnd(xmlData, pos);
            }
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        return false;
    }

    void XMLReader::ParseXMLTag(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        try
        {
            if (xmlData.empty())
                return;
            if (xmlData[pos] != L'<')
                THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"char is not < but " + std::wstring(1, xmlData[pos])));
            
            pos++;
            size_t dataLength = xmlData.length();
            if (pos >= dataLength)
                THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
            
            bool isSkip = xmlData[pos] == L'!' || xmlData[pos] == L'?';

            // TODO: handle <?...?> and <!...>
            if (isSkip) {
                while (pos < dataLength) {
                    if (xmlData[pos] == L'"') {
                        _GetString(xmlData, pos);
                    }
                    if(xmlData[pos] == L'>') {
                        break;
                    }                    
                    pos++;
                }
            } else {
                // tag name
                std::wstring tagName = _GetTag(xmlData, pos);
                if (tagName.empty()) {
                    if (pos + 1 < dataLength && xmlData[pos + 1] == L'/') {
                        pos--;
                        return;
                    }
                    THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
                }
                pos++;
                if (pos >= dataLength)
                    THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"> missing"));

                if (xmlData[pos] == L':') {
                    element.Namespace = tagName;
                    pos++;
                    tagName = _GetTag(xmlData, pos);
                    if (tagName.empty())
                        THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"tag name missing"));
                    pos++;
                    if (pos >= dataLength)
                        THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"> missing"));
                }
                element.Name = tagName;
                while (!_IsNextCharTagEnd(xmlData, pos)) {
                    XMLAttribute attr;
                    attr.Name = _GetTag(xmlData, pos);
                    pos++;
                    if (pos >= dataLength || xmlData[pos] != L'=')
                        THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"= missing"));
                    pos++;
                    attr.Value = _GetString(xmlData, pos);
                    element.Attributes.push_back(attr);
                    pos++;
                }
                // tag end with no ceontent
                if (xmlData[pos - 1] == L'/')
                    return;
                pos++;

                std::wstring endTag = L"</" + (!element.Namespace.empty() ? (element.Namespace + L":") : L"") + element.Name + L">";
                while (pos < dataLength)
                {
                    XMLElement child;
                    ParseXMLElement(xmlData, pos, child);
                    if (!child.Name.empty()) {
                        element.Children.push_back(child);
                        std::wstring childEndTag = L"</" + (!child.Namespace.empty() ? (child.Namespace + L":") : L"") + child.Name + L">";
                        pos++;
                        if (!xmlData.substr(pos).starts_with(childEndTag))
                            THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"end tab " + childEndTag + L" missing"));
                        pos += childEndTag.length();
                    }
                    else
                        element.Text = child.Text;
                    pos++;
                    if (xmlData.substr(pos).starts_with(endTag)) {
                        pos--;
                        break;
                    }
                    if (pos >= dataLength)
                        THROW_EXCEPTION_M(ExceptionType::READER_ERROR, _GetErrorMessage(pos, xmlData[pos], L"end tab " + endTag + L" missing"));
                    pos++;
                }
            }
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }        
    }

    void XMLReader::ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element)
    {
        size_t dataLength = xmlData.length();
        try
        {
            while (pos < dataLength) {
                if (std::iswspace(xmlData[pos])) {
                    // nothing to do
                } else if (pos + 1 < dataLength && xmlData[pos] == L'<' && xmlData[pos + 1] == L'/') {
                    pos--;
                    return;
                }else if (xmlData[pos] == L'<') {
                    ParseXMLTag(xmlData, pos, element);
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
                _ToNextChar(xmlData, pos);
            }
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    XMLElement XMLReader::ParseXML(const std::wstring &xml)
    {
        try
        {
            size_t pos = 0;
            return this->ParseXML(xml, pos);
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        XMLElement empty;
        return empty;
    }

    XMLElement XMLReader::ParseXML(const std::wstring &xml, size_t &pos)
    {
        XMLElement root;
        try
        {;
            ParseXMLElement(xml, pos, root);
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }        
        return root;
    }
}