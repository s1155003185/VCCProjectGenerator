#pragma once

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "xml_builder.hpp"

using namespace vcc;

class VPGCodeReader : public XmlBuilder
{
    GETSET(std::wstring, CommandDelimiter, L"//")
    private:
        VPGCodeReader() = default;
    public:
        VPGCodeReader(std::wstring commandDelimiter);
        
        virtual void ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const override;
        virtual void ParseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const override;

};