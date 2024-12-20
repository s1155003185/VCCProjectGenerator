#pragma once

#include <string>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGActionFileGenerationService
{
    private:
        VPGActionFileGenerationService() = default;
        ~VPGActionFileGenerationService() {}

    public:
        static std::wstring GetActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix);

        static std::vector<std::wstring> GenerateHpp(const LogConfig *logConfig, const VPGEnumClass *enumClass,
            const std::wstring &projectPrefix, const std::wstring &folderPathHpp);
        static std::vector<std::wstring> GenerateCpp(const LogConfig *logConfig, const VPGEnumClass *enumClass,
            const std::wstring &projectPrefix, const std::wstring &foldPathCpp);
};
