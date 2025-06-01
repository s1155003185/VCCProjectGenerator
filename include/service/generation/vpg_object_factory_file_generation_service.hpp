#pragma once

#include <set>
#include <string>

#include "log_config.hpp"

class VPGObjectFactoryFileGenerationService
{
    private:
        VPGObjectFactoryFileGenerationService() = default;
        ~VPGObjectFactoryFileGenerationService() {}

    public:
        static void GenerateHpp(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp);
        static void GenerateCpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix, const std::set<std::wstring> &includeFiles, const std::wstring &filePathCpp, const std::set<std::wstring> &propertyTypes);
};