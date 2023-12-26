#pragma once

#include "log_property.hpp"

#include <string>

using namespace vcc;

enum class VPGProjectType;

class VPGFileUpdateService
{
    public:
        VPGFileUpdateService() {}
        ~VPGFileUpdateService() {}

        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring branch, std::wstring directory);
        static std::wstring UpdateVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory);
};