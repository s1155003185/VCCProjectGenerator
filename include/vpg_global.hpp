#pragma once

#include <string>

enum class VPGProjectType;

class VPGGlobal
{
    public:
        static std::wstring GetDefaultFolder();
        static std::wstring GetProjectName(VPGProjectType projectType);
        static std::wstring GetProjecURL(VPGProjectType projectType);
};