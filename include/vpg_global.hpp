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
        static std::wstring GetVersion();
        static std::wstring GetVccLocalResponseFolder();
        static std::wstring GetVccProjectLocalResponseDirectory(VPGProjectType projectType);
        static std::wstring GetProjectName(VPGProjectType projectType);
        static std::wstring GetProjecURL(VPGProjectType projectType);
        static std::wstring GetVccJsonFileName();

        // c++
        static std::wstring GetCppDefaultIncludePathWindow();
        static std::wstring GetCppDefaultIncludePathLinux();
        static std::wstring GetCppDefaultIncludePathMacOs();

        // VPM - VCC Project Manager
        static std::wstring GetVCCProjectManagerConfigFileName();
        static std::wstring GetVCCProjectManagerConfigFileFullPath();

        // Generator
        static std::shared_ptr<VPGEnumClassReader> GetEnumClassReader();
        static std::shared_ptr<VPGFileGenerationManager> GetFileGenerationManager();

        // helper
        static std::wstring GetConvertedPath(const std::wstring &path);

        // Initialize
        static void Initialize(const std::wstring &projectWorkspace);
};