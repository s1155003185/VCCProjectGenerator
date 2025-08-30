#pragma once

#include <memory>
#include <string>

#include "class_macro.hpp"
#include "xml_builder.hpp"

class VPGCodeReader : public vcc::XmlBuilder
{
    GETSET(std::wstring, CommandDelimiter, L"//")
    private:
        VPGCodeReader() = default;
    public:
        VPGCodeReader(std::wstring commandDelimiter);
        
        virtual void parseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<vcc::Xml> element) const override;
        virtual void parseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<vcc::Xml> element) const override;

};