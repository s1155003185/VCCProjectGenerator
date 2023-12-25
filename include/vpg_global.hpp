#pragma once

#include <string>

enum class VPGProjectType;
class VPGGlobal
{
    public:
        static std::wstring GetVersion();
        static std::wstring GetDefaultFolder();
        static std::wstring GetProjectLocalDirectory(VPGProjectType projectType, std::wstring parentFolder);
        static std::wstring GetProjectName(VPGProjectType projectType);
        static std::wstring GetProjecURL(VPGProjectType projectType);
};