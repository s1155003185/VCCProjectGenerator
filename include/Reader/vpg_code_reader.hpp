#pragma once

#include <string>

#include "class_macro.hpp"
#include "xml_reader.hpp"

using namespace vcc;

class VPGCodeReader : public XMLReader
{
    GET(std::wstring, CommandDelimiter, L"")

    public:
        VPGCodeReader(std::wstring commandDelimiter);
        virtual void ParseXMLElement(const std::wstring &xmlData, size_t &pos, XMLElement &element) override;

};