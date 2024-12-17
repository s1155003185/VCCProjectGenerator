#pragma once

#include <string>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGActionFileGenerationService
{
    private:
        VPGActionFileGenerationService() = default;
        ~VPGActionFileGenerationService() {}

    public:
        static std::wstring GenerateHpp(const LogConfig *logConfig, const std::wstring &filePathHpp, const VPGEnumClass *enumClass);
        static std::wstring GenerateCpp(const LogConfig *logConfig, const std::wstring &filePathCpp, const VPGEnumClass *enumClass);
};
