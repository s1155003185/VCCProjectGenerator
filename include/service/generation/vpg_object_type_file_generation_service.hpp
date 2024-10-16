#pragma once

#include <set>
#include <string>

#include "log_config.hpp"

using namespace vcc;

class VPGObjectTypeFileGenerationService
{
    private:
        VPGObjectTypeFileGenerationService() = default;
        ~VPGObjectTypeFileGenerationService() {}

    public:
        static void Generate(const LogConfig *logConfig, const std::wstring &filePathHpp, const std::set<std::wstring> &propertyTypes);
};