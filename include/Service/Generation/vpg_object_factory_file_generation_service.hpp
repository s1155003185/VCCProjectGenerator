#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGObjectFactoryFileGenerationSerive
{
    private:
        VPGObjectFactoryFileGenerationSerive() = default;
        ~VPGObjectFactoryFileGenerationSerive() {}

    public:
        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &hppFilePath);
        static void GenerateCpp(const LogProperty *logProperty, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles, const std::wstring &cppFilePath, const std::set<std::wstring> &propertyTypes);
};