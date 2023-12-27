#pragma once

#include <string>

enum class VPGProjectType;
class VPGGlobal
{
    public:
        static std::wstring GetVersion();
        static std::wstring GetVccDefaultFolder();
        static std::wstring GetVccProjectDefaultDirectory(VPGProjectType projectType);
        static std::wstring GetProjectName(VPGProjectType projectType);
        static std::wstring GetProjecURL(VPGProjectType projectType);
};