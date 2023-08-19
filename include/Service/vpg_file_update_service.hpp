#pragma once

#include "log_property.hpp"

#include <string>

using namespace vcc;

enum class VPGProjectType;

class VPGFileUpdateService
{
    private:
        static std::wstring _DefaultFolder(LogProperty &logProperty);
        static std::wstring _GetProjectName(VPGProjectType projectType);

        static std::wstring _DownloadVCCResource(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring directory);
        static std::wstring _UpdateVCCResource(LogProperty &logProperty, std::wstring workspace);
    public:
        VPGFileUpdateService() {}
        ~VPGFileUpdateService() {}

        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory);
        static std::wstring UpdateVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory);
};