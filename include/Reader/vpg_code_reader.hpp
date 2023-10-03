#pragma once

#include "xml_reader.hpp"

using namespace vcc;

class VPGCodeReader : public XMLReader
{
    public:
        XMLElement ParseCode(const std::wstring &xml);
};