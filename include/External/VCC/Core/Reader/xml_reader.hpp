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
            virtual std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);
            virtual std::wstring _GetString(const std::wstring &xmlData, size_t &pos);
            virtual std::wstring _GetTag(const std::wstring &xmlData, size_t &pos);
            virtual bool _IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos);
            virtual bool _IsXMLHeader(const std::wstring &xmlData, size_t &pos);
            virtual void _ParseXMLHeader(const std::wstring &xmlData, size_t &pos); // TODO: handle <?...?> and <!...>
            
        public:
            XMLReader() = default;
            virtual ~XMLReader() {}

            // tag
            virtual bool ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, XMLElement &element); // ture if have tag end
            virtual void ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, XMLElement &element);
            virtual void RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, XMLElement &element);

            // content
            virtual void ParseXMLTag(const std::wstring &xmlData, size_t &pos, XMLElement &element);
            virtual void ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element);

            virtual XMLElement Parse(const std::wstring &xml);
            virtual XMLElement Parse(const std::wstring &xml, size_t &pos);
    };
}