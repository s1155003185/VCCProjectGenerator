#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGPropertyAccessorFactoryFileGenerationService
{
    private:
        VPGPropertyAccessorFactoryFileGenerationService() = default;
        ~VPGPropertyAccessorFactoryFileGenerationService() {}

    public:
        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &filePathHpp);
        static void GenerateCpp(const LogProperty *logProperty, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles, const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes);
};