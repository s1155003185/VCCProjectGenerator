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
        static std::wstring GenerateHpp(const LogConfig *logConfig, const std::wstring &folderPathHpp, const VPGEnumClass *enumClass);
        static std::wstring GenerateCpp(const LogConfig *logConfig, const std::wstring &foldPathCpp, const VPGEnumClass *enumClass);
};
