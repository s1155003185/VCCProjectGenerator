#pragma once

#include <string>

#include "class_macro.hpp"
#include "xml_reader.hpp"

using namespace vcc;

class VPGCodeReader : public XMLReader
{
    GETSET(std::wstring, CommandDelimiter, L"//")
    private:
        VPGCodeReader() = default;
    public:
        VPGCodeReader(std::wstring commandDelimiter);
        
        virtual void ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, XMLElement &element) override;
        virtual void ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element) override;

};