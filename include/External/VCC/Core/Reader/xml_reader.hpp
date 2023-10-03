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
    };

    class XMLReader
    {
        private:
            std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);
            std::wstring _GetString(const std::wstring &xmlData, size_t &pos);
            std::wstring _GetTag(const std::wstring &xmlData, size_t &pos);
            void _ToNextChar(const std::wstring &xmlData, size_t &pos);
            bool _IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos);
        public:
            void ParseXMLTag(const std::wstring &xmlData, size_t &pos, XMLElement &element);
            void ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element);

            XMLElement ParseXML(const std::wstring &xml);
            XMLElement ParseXML(const std::wstring &xml, size_t &pos);
    };
}