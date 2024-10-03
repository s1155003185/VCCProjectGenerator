#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGObjectFactoryFileGenerationService
{
    private:
        VPGObjectFactoryFileGenerationService() = default;
        ~VPGObjectFactoryFileGenerationService() {}

    public:
        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &filePathHpp);
        static void GenerateCpp(const LogProperty *logProperty, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles, const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes);
};