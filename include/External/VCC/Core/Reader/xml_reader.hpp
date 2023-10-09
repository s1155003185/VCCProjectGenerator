#pragma once

#include <map>
#include <string>
#include <vector>

namespace vcc
{
    // TODO: Not Complete
    struct XMLAttribute 
    {
        std::wstring Name;
        std::wstring Value;
    };

    struct XMLElement {
        std::wstring Namespace;
        std::wstring Name;
        std::vector<XMLAttribute> Attributes;
        std::vector<XMLElement> Children;
        std::wstring Text;
        std::wstring FullText;
    };

    class XMLReader
    {
        protected:
            virtual std::wstring GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);
            virtual std::wstring GetString(const std::wstring &xmlData, size_t &pos);
            virtual std::wstring GetTag(const std::wstring &xmlData, size_t &pos);
            virtual bool IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos);
            virtual bool IsXMLHeader(const std::wstring &xmlData, size_t &pos);
            virtual void ParseXMLHeader(const std::wstring &xmlData, size_t &pos); // TODO: handle <?...?> and <!...>
            
        public:
            XMLReader() = default;
            virtual ~XMLReader() {}

            // header
            virtual bool ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, XMLElement &element); // ture if have tag end
            
            // tag
            virtual void ParseXMLTag(const std::wstring &xmlData, size_t &pos, XMLElement &element);
            virtual void ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element);

            virtual XMLElement Parse(const std::wstring &xml);
            virtual XMLElement Parse(const std::wstring &xml, size_t &pos);
    };
}