#pragma once

#include <map>
#include <set>
#include <string>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

class VPGActionFileGenerationService
{
    private:
        VPGActionFileGenerationService() = default;
        ~VPGActionFileGenerationService() {}

    public:
        static void GenerateHpp(const vcc::LogConfig *logConfig,
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const VPGEnumClass *enumClass,
            const std::wstring &projectPrefix, const std::wstring &folderPathHpp,
            std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles);
            
        static void GenerateCpp(const vcc::LogConfig *logConfig, 
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const VPGEnumClass *enumClass,
            const std::wstring &projectPrefix, const std::wstring &foldPathCpp,
            std::map<std::wstring, std::wstring> &actionClassList, std::set<std::wstring> &globalSystemIncludeFiles, std::set<std::wstring> &globalCustomIncludeFiles);
};
