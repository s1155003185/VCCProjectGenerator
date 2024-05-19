#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGObjectTypeFileGenerationSerive
{
    private:
        VPGObjectTypeFileGenerationSerive() = default;
        ~VPGObjectTypeFileGenerationSerive() {}

    public:
        static void Generate(const LogProperty *logProperty, const std::wstring &hppFilePath, const std::set<std::wstring> &propertyTypes);
};