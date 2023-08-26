#pragma once

#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGFileSyncService
{
    public:
        VPGFileSyncService() {}
        ~VPGFileSyncService() {}
        static void CopyFile(LogProperty &logProperty, const std::wstring &sourcePath, const std::wstring &targetPath);
};