#pragma once

#include <string>

#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_manager.hpp"

enum class VPGProjectType;
class VPGGlobal
{
    private:
        static std::shared_ptr<VPGEnumClassReader> _EnumClassReader;
        static std::shared_ptr<VPGFileGenerationManager> _FileGenerationManager;

    public:
        static std::wstring getVersion();
        static std::wstring getVccLocalResponseFolder();
        static std::wstring getVccProjectLocalResponseDirectory(VPGProjectType projectType);
        static std::wstring getProjectName(VPGProjectType projectType);
        static std::wstring getProjecURL(VPGProjectType projectType);
        static std::wstring getVccJsonFileName();

        // c++
        static std::wstring getCppDefaultIncludePathWindow();
        static std::wstring getCppDefaultIncludePathLinux();
        static std::wstring getCppDefaultIncludePathMacOs();

        // VPM - VCC Project Manager
        static std::wstring getVCCProjectManagerConfigFileName();
        static std::wstring getVCCProjectManagerConfigFileFullPath();

        // Generator
        static std::shared_ptr<VPGEnumClassReader> getEnumClassReader();
        static std::shared_ptr<VPGFileGenerationManager> getFileGenerationManager();

        // helper
        static std::wstring getConvertedPath(const std::wstring &path);

        // Initialize
        static void initialize(const std::wstring &projectWorkspace);
};