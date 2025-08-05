#pragma once

#include <set>
#include <string>

#include "log_config.hpp"

class VPGObjectTypeFileGenerationService
{
    private:
        VPGObjectTypeFileGenerationService() = default;
        ~VPGObjectTypeFileGenerationService() {}

    public:
        static void generate(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp, const std::set<std::wstring> &propertyTypes);
};